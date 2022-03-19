
chick_4 = {

    new = function()
        
        local self = Instantiate(chick_1, ChickLuaData)
        
        self.events = {
            move = nil,
        }

        OutLog("chick_1 is created.\n")

        return self
    end,

    update = function(self)
        PunicatLuaData.update(self)

        if (self.events.move==nil) then
            self.events.move = coroutine.create(ChickLuaData.move)
            coroutine.resume(self.events.move, self)
        end

    end,


    talk = function (self, e)
        local w = MessageWindow.open()

        w:streamText("ぴよ、ぴよ..")
        while w:isRunning() do Yield() end

        w:streamText("ママのところに帰る")
        while w:isRunning() do Yield() end

        w:close()
    end
}




