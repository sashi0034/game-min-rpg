
MessageWindowLuaData = {

    new = function()
        
        local self = Instantiate(MessageWindowLuaData, IngameEventBase)
        
        self.events = {}

        self.centerY = 192
        self.width = 280
        self.height = 80
        self.paddingX = 24
        self.paddingY = 16
        self.lineMargin = 16
        self.letterReadMinInterval = 5
        self.letterReadInterval = 50

        self.shakeIntensity = 20
        self.flashInterval = 20
        self.flashFrame = 2

        OutLog("MessageWindow in Lua is initilaized.\n")
        return self
    end,

    update = function(self)
        IngameEventBase.update(self)
    end,
}








