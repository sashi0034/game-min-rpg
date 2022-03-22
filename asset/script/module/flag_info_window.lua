
FlagInfoWindowLuaData = {

    new = function()
        
        local self = Instantiate(FlagInfoWindowLuaData, IngameEventBase)
        
        self.events = {}

        self.opacity = 200
        self.centerX = 80
        self.centerY = 80
        self.width = 144
        self.height = 12
        self.paddingLeft = 12;
        self.paddingTop = 2;

        self.textForegroundColor = {32, 32, 32}
        self.textEdgegroundColor = {255, 255, 255}

        return self
    end,

    update = function(self)
        IngameEventBase.update(self)
    end,
}








