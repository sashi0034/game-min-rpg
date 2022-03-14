

-- class Punicat
SlimeLuaData = {

    new = function()
        
        local self = Instantiate(SlimeLuaData, IngameEventBase)
        
        self.vel = 50
        self.frameInterval = 200

        return self
    end,

    update = function(self)
        IngameEventBase.update(self)

        if (self.events.talk~=nil) then
            self.pausedEvents.move = true
        else
            self.pausedEvents.move = nil
        end
    end,
}








