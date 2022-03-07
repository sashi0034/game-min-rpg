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
            end
        end

        self.time = self.time+Time.deltaMilli()
        return 0
    end,

    doWait = function (time)
        local t = 0
        while t<time do 
            t = t + Time.deltaSec()
            Yield(true)
        end
        return false
    end,
}

