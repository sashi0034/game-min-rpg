-- class IngameEventBase
IngameEventBase = {
    new = function()
        local self = Instantiate(IngameEventBase)
        
        self.time = 0
        self.events = {}
        
        return self
    end,

    
    update = function(self)
        for key, value in pairs(self.events) do
            if (coroutine.status(value))~="dead" then
                coroutine.resume(value)
            else
                self.events[key] = nil
            end
        end

        self.time = self.time+Time.deltaMilli()
        return 0
    end,

    doSleep = function (time)
        local t = 0
        while t<time do 
            t = t + Time.deltaSec()
            Yield(true)
        end
        return false
    end,

    eventDrive = function (self, key, e)
        if (self.key==nil) then
            ErrLog("Event function: "..key.." is not exit.\n");
            return
        elseif (self.events[key]~=nil) then
            ErrLog("Event function: "..key.." is already running.\n")
            return
        end
        
        self.events[key] = coroutine.create(self.key)
        coroutine.resume(self.events[key], self, e)
    end
}

