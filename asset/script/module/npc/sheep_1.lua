


sheep_1 = {

    new = function()
        
        local self = Instantiate(cat_1, PunicatLuaData)
        
        self.events = {
            move = nil,
        }
        self.vel = 25

        return self
    end,

    update = function(self)
        PunicatLuaData.update(self)

        if (self.events.move==nil) then
            self.events.move = coroutine.create(self.move)
            coroutine.resume(self.events.move, self)
        end

    end,

    move = function (self)
        local x, y, c
        x=self.getX(); y=self.getY();

        if (math.random(1, 5)>1) then
            local e = MapEventManager.getUnique("cat_1_there")

            while self.doMove(e.x, e.y) do Yield() end
    
            c = coroutine.create( self.doSleep )
            while coroutine.resume(c, 2.5) do Yield() end
            
            while self.doMove(e.x-16, e.y) do Yield() end
            while self.doMove(e.x-16, e.y-32) do Yield() end
            while self.doMove(e.x-16, e.y+16) do Yield() end
            while self.doMove(e.x-16, e.y) do Yield() end
            while self.doMove(e.x-32, e.y) do Yield() end
        else
            local e1, e2
            e1 = MapEventManager.getUnique("cat_1_move_1")
            e2 = MapEventManager.getUnique("cat_1_move_2")
            while self.doMove(e1.x, e1.y) do Yield() end
            while self.doMove(e2.x, e2.y) do Yield() end

            c = coroutine.create( self.doSleep )
            while coroutine.resume(c, 1.5) do Yield() end

            while self.doMove(e1.x, e1.y) do Yield() end
        end
    end,

    talk = function (self, e)
        local w = MessageWindow.open()

        w:streamText([[にゃんにゃん]])
        while w:isRunning() do Yield() end

        w:streamText("\n"..[[ネコはつらいにゃん]])
        while w:isRunning() do Yield() end

        local s = SelectionWindow.open({[[はい]], [[いいえ]], [[にゃん]]})
        while s:isRunning() do Yield() end
        local index=s:selectedIndex()
        s:close()

        if index==0 then
            w:streamText("\n"..[[世の中がもっと]].."\n"..[[ハッピーになってほしいにゃん]])
            while w:isRunning() do Yield() end
        elseif index==1 then
            w:streamText("\n"..[[うう、頑張って生きるにゃん]])
            while w:isRunning() do Yield() end
        elseif index==2 then
            w:streamText("\n"..[[にゃおん、にゃ～ん]])
            while w:isRunning() do Yield() end
        end

        w:close()
    end
    
}








