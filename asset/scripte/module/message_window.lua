
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
        self.letterReadInterval = 50

        OutLog("MessageWindow in Lua is initilaized.\n")
        return self
    end,

    update = function(self)
        IngameEventBase.update(self)

        if (self.events.move==nil) then
            if (self.isFixed==false and self.doWaitForMove()) then
                --OutLog("Player is prepareing for move.\n")
                self.events.move = coroutine.create(self.move)
                coroutine.resume(self.events.move, self)
            end
        end
    end,

    move = function (self)
        while self.doMove() do Yield() end
        --OutLog("Player move is completed.\n")
    end,
}








