
Yield = coroutine.yield

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


function GetRandomCrosswise()
    local r = math.random(0, 3)
    if (r==0) then 
        return {x= -1, y=1}
    elseif r==1 then
        return {x= 1, y=1}
    elseif r==2 then
        return {x= -1, y=-1}
    elseif r==3 then
        return {x= 1, y=-1}
    end
end

function GetRandomCrosswise16()
    local r = math.random(0, 3)
    if (r==0) then 
        return {x= -16, y=0}
    elseif r==1 then
        return {x= 16, y=0}
    elseif r==2 then
        return {x= 0, y=-16}
    elseif r==3 then
        return {x= 0, y=16}
    end
end
