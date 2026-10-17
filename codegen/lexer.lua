local function fail(message)
    error(message, 0)
end

-- Lexer ----------------------------------------------------------------------
local function tokenize(source)
    local tokens, i = {}, 1
    while i <= #source do
        local rest = source:sub(i)
        local whitespace = rest:match("^%s+")
        local line_comment = rest:match("^//[^\n]*")
        local block_comment = rest:match("^/%*.-%*/")
        if whitespace then
            i = i + #whitespace
        elseif line_comment then
            i = i + #line_comment
        elseif block_comment then
            i = i + #block_comment
        elseif rest:sub(1, 2) == "/*" then
            fail("unterminated block comment")
        else
            local token
            if rest:sub(1, 1) == '"' then
                local j, escaped = 2, false
                while j <= #rest do
                    local ch = rest:sub(j, j)
                    if ch == '"' and not escaped then break end
                    if ch == "\\" and not escaped then escaped = true else escaped = false end
                    j = j + 1
                end
                if j > #rest then fail("unterminated string literal") end
                token = rest:sub(1, j)
            end
            if not token then
                for _, op in ipairs({"->", "::", ">=", "<=", "==", "!=", "&&"}) do
                    if rest:sub(1, #op) == op then token = op; break end
                end
            end
            token = token or rest:match("^[%a_][%w_]*") or rest:match("^%d+") or rest:sub(1, 1)
            tokens[#tokens + 1] = token
            i = i + #token
        end
    end
    return tokens
end

return tokenize
