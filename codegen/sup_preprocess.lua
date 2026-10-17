#!/usr/bin/env lua
-- Copy a C++ source tree while expanding class [[sup]] annotations.

local function fail(message) error("sup-preprocess: " .. message, 0) end
local root, output, input
local i = 1
while i <= #arg do
    if arg[i] == "--root" then root = arg[i + 1]; i = i + 2
    elseif arg[i] == "--input" then input = arg[i + 1]; i = i + 2
    elseif arg[i] == "-o" or arg[i] == "--output" then output = arg[i + 1]; i = i + 2
    else fail("unexpected argument '" .. arg[i] .. "'") end
end
if not output or (not root and not input) or (root and input) then
    fail("usage: sup_preprocess.lua --input SOURCE -o OUTPUT, or --root SOURCE_DIR -o OUTPUT_DIR")
end
if root then root = root:gsub("/$", "") end

local function read(path)
    local f, err = io.open(path, "rb"); if not f then fail(err) end
    local s = f:read("*a"); f:close(); return s
end
local function write(path, s)
    local parent = path:match("^(.*)/[^/]+$")
    if parent then os.execute("mkdir -p " .. string.format("%q", parent)) end
    local f, err = io.open(path, "wb"); if not f then fail(err) end
    f:write(s); f:close()
end

local function preprocess(source, destination)
    local text = read(source)
    local pos = 1
    while true do
        local start, after_class = text:find("class%s+", pos)
        if not start then break end
        local brace = text:find("{", after_class, true)
        if not brace then break end
        local semicolon = text:find(";", after_class, true)
        if semicolon and semicolon < brace then
            pos = semicolon + 1
        else
          local declaration = text:sub(after_class, brace - 1)
          if declaration:find("%[%[%s*sup%s*%]%]") then
            local base_list = declaration:match("%s:%s*(.-)%s*$")
            local base = base_list and base_list:match("^%s*(.-)%s*,") or base_list
            if base then
                base = base:gsub("^%s*", ""):gsub("%s*$", "")
                -- Discard the access specifier attached to the first base.
                base = base:gsub("^public%s+", ""):gsub("^protected%s+", ""):gsub("^private%s+", "")
                base = base:gsub("^virtual%s+", "")
                if base == "" then fail("empty base in " .. source) end
                local class_name, tail = declaration:match("%[%[%s*sup%s*%]%]%s*([%w_:]+)(.*)")
                if not class_name then fail("could not read class name in " .. source) end
                local class_type = class_name
                local template_args = tail:match("^%s*(<.*>)")
                if template_args then
                    local depth, finish = 0, nil
                    for offset = 1, #template_args do
                        local char = template_args:sub(offset, offset)
                        if char == "<" then depth = depth + 1
                        elseif char == ">" then
                            depth = depth - 1
                            if depth == 0 then finish = offset; break end
                        end
                    end
                    if finish then class_type = class_type .. template_args:sub(1, finish) end
                end
                local method = "\npublic:\n\tvirtual bool isA(type_id const &what) const override { return what == type_id(typeid(" .. class_type .. ")) || this->" .. base .. "::isA(what); }\n"
                text = text:sub(1, brace) .. method .. text:sub(brace + 1)
                pos = brace + #method + 1
            else
                fail("class marked [[sup]] has no base class in " .. source)
            end
          else
              pos = brace + 1
          end
        end
    end
    write(destination, text:gsub("%[%[%s*sup%s*%]%]", ""))
end

if input then
    preprocess(input, output)
else
    -- Walk all conventional C++ translation units and headers under the root.
    local pipe = assert(io.popen("find " .. string.format("%q", root) .. " -type f \\( -name '*.cpp' -o -name '*.cc' -o -name '*.cxx' -o -name '*.h' -o -name '*.hpp' -o -name '*.hh' -o -name '*.hxx' \\) -print"))
    for source in pipe:lines() do
        local relative = source:sub(#root + 2)
        preprocess(source, output .. "/" .. relative)
    end
    pipe:close()
end
