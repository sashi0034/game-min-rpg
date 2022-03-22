
LimitTimeWindowLuaData = {

    new = function()
        
        local self = Instantiate(LimitTimeWindowLuaData, IngameEventBase)
        
        self.events = {}

        self.opacity = 200
        self.centerX = 336
        self.centerY = 16
        self.width = 64
        self.height = 16
        self.paddingLeft = 24;
        self.paddingTop = 2;

        self.textForegroundColor = {244, 132, 50}
        self.textEdgegroundColor = {92, 63, 55}

        return self
    end,

    update = function(self)
        IngameEventBase.update(self)
    end,
}








