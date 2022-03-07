
Yield = coroutine.yield

COROUTINE_DEAD = coroutine.create(function () end)
coroutine.resume(COROUTINE_DEAD)

function Instantiate(class, super, ...)
    local self = (super and super.new(...) or {})
    setmetatable(self, {__index = class})
    setmetatable(class, {__index = super})
    return self
end

