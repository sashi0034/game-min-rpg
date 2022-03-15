
MapEventManager = {
    new = function()
        
        local self = Instantiate(MapEventManager, IngameEventBase)
        
        self.events = {}

        self.vel = 50
        self.isFixed = false

        for key, value in pairs(self.staticEvents) do
            self.events[key] = coroutine.create(value)
            coroutine.resume(self.events[key], self)
        end

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
    staticEvents = {},

    addMapEvents = function (self, table)
        for key, value in pairs(table) do
            self.mapEvents[key] = value
        end
    end,

    addStaticEvents = function (self, table)
        for key, value in pairs(table) do
            self.staticEvents[key] = value
        end
    end,
}











