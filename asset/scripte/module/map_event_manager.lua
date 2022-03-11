
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
            ErrLog("Event function: "..key.."is not exit.\n");
            return
        elseif (self.events[key]~=nil) then
            ErrLog("Event function: "..key.."is already running.\n")
            return
        end
        
        self.events[key] = coroutine.create(self.mapEvents[key])
        coroutine.resume(self.events[key], self, e)
    end,

    mapEvents = {},

    addMapEvents = function (self, table)
        for key, value in pairs(table) do
            self.mapEvents[key] = value
        end
    end,
}











