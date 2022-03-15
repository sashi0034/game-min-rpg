
SelectionWindowLuaData = {

    new = function()
        
        local self = Instantiate(SelectionWindowLuaData, IngameEventBase)
        
        self.events = {}

        self.centerX = 288
        self.bottomY = 144 + 32
        self.width = 96
        self.lineHeight = 24
        self.cursolLeftX = 4
        self.paddingX = 32
        self.paddingY = 10
        self.lineMargin = 16
        self.letterReadMinInterval = 5
        self.letterReadInterval = 50
        self.buttonIntervalFirst = 21
        self.buttonIntervalSecond = 6

        OutLog("MessageWindow in Lua is initilaized.\n")
        return self
    end,

    update = function(self)
        IngameEventBase.update(self)
    end,
}








