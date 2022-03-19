

ChickenLuaData = {

    new = function()
        
        local self = Instantiate(ChickenLuaData, IngameEventBase)
        
        self.vel = 50
        self.frameInterval = 100

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

