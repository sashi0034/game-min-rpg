
skull_last = {

    new = function()
        
        local self = Instantiate(skull_last, SkullLuaData)
        
        self.events = {
            move = nil,
        }
        self.vel = 25
        self.frameInterval = 200

        return self
    end,

    update = function(self)
        SkullLuaData.update(self)

        if (self.events.move==nil) then
            self.events.move = coroutine.create(self.move)
            coroutine.resume(self.events.move, self)
        end

    end,

    move = function (self)
        local x, y, c
        x=self.getX(); y=self.getY();

        while self.doMove(x-16, y) do Yield() end
        while self.doMove(x+16, y) do Yield() end
        while self.doMove(x, y) do Yield() end
        self.awaits.sleep(4.0)
        while self.doMove(x, y-16) do Yield() end
        while self.doMove(x, y+16) do Yield() end
        while self.doMove(x, y) do Yield() end
        self.awaits.sleep(4.0)

    end,

    talk = function (self, e)
        local w = MessageWindow.open()

        self.awaits.streamText(w, [[にゃんにゃん]])
        local index, word = self.awaits.selectionWindow({"はい", "いいえ"})

        if index==0 then
            self.awaits.streamText(w, [[まじかよ..]])
        else
            self.awaits.streamText(w, [[なんでやねん]])
        end

        w:close()
    end
    
}








