local function fail(message)
    error(message, 0)
end

-- C++ output -----------------------------------------------------------------
local function quote(value) return '"' .. value:gsub("\\", "\\\\"):gsub('"', '\\"') .. '"' end
local function join(values, sep) return table.concat(values, sep or ", ") end
local function overload(values) return #values == 1 and values[1] or "overload(" .. join(values) .. ")" end
local function function_pointer(method)
    return "static_cast<" .. method.ret .. "(*) (" .. join(method.args) .. ")>(" .. method.alias .. ")"
end
local function class_method_pointer(ref, method)
    return "static_cast<" .. method.ret .. " (" .. ref .. "::*) (" .. join(method.args) .. ")>(&" .. ref .. "::" .. method.alias .. ")"
end
local function emit_namespace(item, out)
    out[#out + 1] = "lua[" .. quote(item.name) .. "] = lua.create_table();"
    local groups, order = {}, {}
    for _, member in ipairs(item.body) do
        if member.kind == "method" or member.kind == "value" then
            local expr = member.kind == "method" and function_pointer(member) or member.expr
            if not groups[member.name] then groups[member.name] = {}; order[#order + 1] = member.name end
            groups[member.name][#groups[member.name] + 1] = expr
        end
    end
    for _, name in ipairs(order) do out[#out + 1] = "lua[" .. quote(item.name) .. "][" .. quote(name) .. "] = " .. overload(groups[name]) .. ";" end
end
local function emit_global(item, out)
    local groups, order = {}, {}
    for _, member in ipairs(item.body) do
        if member.kind ~= "method" and member.kind ~= "value" then fail(member.kind .. " is not valid in a global block") end
        local expr = member.kind == "method" and function_pointer(member) or member.expr
        if not groups[member.name] then groups[member.name] = {}; order[#order + 1] = member.name end
        groups[member.name][#groups[member.name] + 1] = expr
    end
    for _, name in ipairs(order) do out[#out + 1] = "lua[" .. quote(name) .. "] = " .. overload(groups[name]) .. ";" end
end
local function emit_class(item, out)
    local args, methods, method_order, constructors = {}, {}, {}, {}
    for _, member in ipairs(item.body) do
        if member.kind == "new" then
            local names, decl = {}, {}
            for i, typ in ipairs(member.args) do names[i] = "arg" .. (i - 1); decl[i] = typ .. " " .. names[i] end
            local lambda = "[](" .. join(decl) .. "){return new " .. item.cpp .. "(" .. join(names) .. ");}"
            constructors[#constructors + 1] = lambda
        elseif member.kind == "method" then
            if not methods[member.name] then methods[member.name] = {}; method_order[#method_order + 1] = member.name end
            methods[member.name][#methods[member.name] + 1] = class_method_pointer(item.cpp, member)
        elseif member.kind == "factory" then args[#args + 1] = quote(member.name) .. ", factories(&" .. item.cpp .. "::" .. member.alias .. ")"
        elseif member.kind == "value" then args[#args + 1] = quote(member.name) .. ", " .. member.expr end
    end
    if #constructors > 0 then
        local factory = #constructors == 1 and "factories(" .. constructors[1] .. ")" or "overload(" .. join(constructors) .. ")"
        table.insert(args, 1, quote("new") .. ", " .. factory)
    end
    for _, name in ipairs(method_order) do args[#args + 1] = quote(name) .. ", " .. overload(methods[name]) end
    if #item.bases > 0 then args[#args + 1] = "sol::base_classes, sol::bases<" .. join(item.bases) .. ">()" end
    out[#out + 1] = "lua.new_usertype<" .. item.cpp .. ">(" .. quote(item.name) .. (#args > 0 and ",\n    " .. join(args, ",\n    ") or "") .. ");"
    for _, member in ipairs(item.body) do if member.kind == "static" then out[#out + 1] = "lua[" .. quote(item.name) .. "][" .. quote(member.name) .. "] = " .. member.expr .. ";" end end
end
local function emit(items)
    local out = {}
    for _, item in ipairs(items) do
        if item.kind == "include" then out[#out + 1] = "#include " .. item.header
        elseif item.kind == "using" then out[#out + 1] = "using namespace " .. item.name .. ";"
        elseif item.kind == "namespace" then emit_namespace(item, out)
        elseif item.kind == "global" then emit_global(item, out)
        elseif item.kind == "opaque" then out[#out + 1] = "lua.new_usertype<" .. item.cpp .. ">( " .. quote(item.name) .. ");"
        elseif item.kind == "class" then emit_class(item, out) end
    end
    return table.concat(out, "\n\n") .. "\n"
end

return emit
