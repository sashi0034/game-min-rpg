

GameControllerLuaData = {

    new = function()
        
        local self = Instantiate(GameControllerLuaData, IngameEventBase)
        
        self.events = {}



        return self
    end,

    update = function(self)
        IngameEventBase.update(self)
    end,
}








