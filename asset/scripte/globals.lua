
Yield = coroutine.yield

COROUTINE_DEAD = coroutine.create(function () end)
coroutine.resume(COROUTINE_DEAD)

function Instantiate(class, super, ...)
    local self = (super and super.new(...) or {})
    setmetatable(self, {__index = class})
    setmetatable(class, {__index = super})
    return self
end


function OutLog(str)
    str = str or ""
    local log = "\n [OUT] " .. debug.getinfo(2).source .. " ** ".. debug.getinfo(2).currentline .. "\n" .. str
    Cout(tostring(log))
end

function ErrLog(str)
    str = str or ""
    local log = "\n [ERR] " .. debug.getinfo(2).source .. " ** ".. debug.getinfo(2).currentline .. "\n" .. str
    Cerr(tostring(log))
end

