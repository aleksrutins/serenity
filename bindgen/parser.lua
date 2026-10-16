local tokenize = require("lexer")

local function fail(message)
    error(message, 0)
end

-- Parser ---------------------------------------------------------------------
local Parser = {}
Parser.__index = Parser
function Parser.new(source) return setmetatable({tokens = tokenize(source), pos = 1}, Parser) end
function Parser:peek() return self.tokens[self.pos] or "" end
function Parser:take(expected)
    local token = self:peek()
    if token == "" then fail("unexpected end of input") end
    if expected and token ~= expected then fail("expected '" .. expected .. "', got '" .. token .. "'") end
    self.pos = self.pos + 1
    return token
end
function Parser:until_token(stops)
    local parts, depth = {}, 0
    while self:peek() ~= "" do
        local token = self:peek()
        if depth == 0 and stops[token] then break end
        self:take(); parts[#parts + 1] = token
        if token == "(" or token == "<" or token == "[" or token == "{" then depth = depth + 1
        elseif token == ")" or token == ">" or token == "]" or token == "}" then depth = depth - 1 end
    end
    local result = ""
    for _, token in ipairs(parts) do
        if result:match("[%w_]$") and token:match("^[%w_]") then result = result .. " " end
        result = result .. token
    end
    return result
end
function Parser:arguments()
    self:take("(")
    local args = {}
    while self:peek() ~= ")" do
        args[#args + 1] = self:until_token({[","] = true, [")"] = true})
        if self:peek() ~= "," then break end
        self:take(",")
    end
    self:take(")")
    return args
end
function Parser:block(terminator)
    local items = {}
    while self:peek() ~= terminator do
        if self:peek() == "" then fail("missing '" .. terminator .. "'") end
        if self:peek() == ";" then self:take(";") else items[#items + 1] = self:member() end
    end
    return items
end
function Parser:member()
    local kind = self:peek()
    if kind == "factory" then
        self:take(); local name = self:take(); local args = self:arguments(); local alias = name
        if self:peek() == "->" then self:take(); alias = self:until_token({[";"] = true}) end
        self:take(";"); return {kind="factory", name=name, args=args, alias=alias}
    elseif kind == "new" then
        self:take(); local args = self:arguments(); local alias = "new"
        if self:peek() == "->" then self:take(); alias = self:until_token({[";"] = true}) end
        self:take(";"); return {kind="new", args=args, alias=alias}
    elseif kind == "static" then
        self:take(); local name = self:take(); self:take("->")
        local expr = self:until_token({[";"] = true}); self:take(";")
        return {kind="static", name=name, expr=expr}
    end
    local declaration = self:until_token({["("]=true, [";"]=true, ["->"]=true})
    if self:peek() == "(" then
        local ret, name = declaration:match("^(.-)([%a_][%w_]*)$")
        if not name then fail("invalid function declaration '" .. declaration .. "'") end
        local args = self:arguments(); local alias = name
        if self:peek() == "->" then self:take(); alias = self:until_token({[";"]=true}) end
        self:take(";"); return {kind="method", ret=ret:gsub("%s+$", ""), name=name, args=args, alias=alias}
    end
    local alias = declaration
    if self:peek() == "->" then self:take(); alias = self:until_token({[";"]=true}) end
    self:take(";"); return {kind="value", name=declaration, expr=alias}
end
function Parser:declaration()
    if self:peek() == "include" then
        self:take(); local header = self:until_token({[";"]=true}); self:take(";"); return {kind="include", header=header}
    elseif self:peek() == "using" then
        self:take(); self:take("namespace"); local name = self:until_token({[";"]=true}); self:take(";"); return {kind="using", name=name}
    end
    local opaque = false
    if self:peek() == "opaque" then self:take(); opaque = true end
    local kind = self:take()
    if kind == "namespace" then
        local name = self:take(); self:take("{"); local body = self:block("}"); self:take("}")
        return {kind="namespace", name=name, body=body}
    elseif kind == "global" then
        self:take("{"); local body = self:block("}"); self:take("}"); return {kind="global", body=body}
    elseif kind ~= "class" then fail("expected namespace or class, got '" .. kind .. "'") end
    local name, cpp = self:take(), nil; cpp = name
    if self:peek() == "(" then self:take(); cpp = self:until_token({[")"] = true}); self:take(")") end
    local bases = {}
    if self:peek() == ":" then
        self:take(":")
        repeat bases[#bases + 1] = self:until_token({[","]=true, ["{"]=true}); if self:peek() ~= "," then break end; self:take(",") until false
    end
    if opaque then self:take(";"); return {kind="opaque", name=name, cpp=cpp, bases=bases} end
    self:take("{"); local body = self:block("}"); self:take("}"); if self:peek() == ";" then self:take(";") end
    return {kind="class", name=name, cpp=cpp, bases=bases, body=body}
end
function Parser:parse()
    local items = {}
    while self:peek() ~= "" do
        if self:peek() == ";" then self:take(";") else items[#items + 1] = self:declaration() end
    end
    return items
end

return Parser
