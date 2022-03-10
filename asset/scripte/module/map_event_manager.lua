
MapEventManager = {
    new = function()
        
        local self = Instantiate(MapEventManager, IngameEventBase)
        
        self.events = {}

        self.vel = 50
        self.isFixed = false

        OutLog("MapEventManager in Lua is initilaized.\n")
        return self
    end,

    update = function(self)
        IngameEventBase.update(self)
    end,

    trigger = function (self, key, e)
        if (self.mapEvents[key]==nil) then
            OutLog("dsfsfsdfs")
            ErrLog("Event function: "..key.."is not exit.");
            return
        end
        
        self.events[key] = coroutine.create(self.mapEvents[key])
        coroutine.resume(self.events[key], self, e)
    end,


    mapEvents = {
        house_1 = function (self, e)
            OutLog("Enter house.\n")
            local c = coroutine.create( self.doSleep )
            while coroutine.resume(c, 2.5) do Yield() end
            OutLog("OK.\n")
        end
    },
}











