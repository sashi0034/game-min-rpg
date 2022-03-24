

sheep_1 = {

    new = function()
        
        local self = Instantiate(sheep_1, SheepLuaData)
        
        self.events = {
            move = nil,
        }
        self.vel = 30

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

        local e1, e2, e3, e4
        e1 = MapEventManager.getUnique("sheep_1_move_1")
        e2 = MapEventManager.getUnique("sheep_1_move_2")
        e3 = MapEventManager.getUnique("sheep_1_move_3")
        e4 = MapEventManager.getUnique("sheep_1_move_4")

        while self.doMove(e1.x, e1.y) do Yield() end
        while self.doMove(e2.x, e2.y) do Yield() end
        while self.doMove(e3.x, e3.y) do Yield() end
        while self.doMove(e4.x, e4.y) do Yield() end

        c = coroutine.create( self.doSleep )
        while coroutine.resume(c, 1.5) do Yield() end

        while self.doMove(e3.x, e3.y) do Yield() end

        while self.doMove(e2.x, e2.y) do Yield() end
        while self.doMove(e1.x, e1.y) do Yield() end

        c = coroutine.create( self.doSleep )
        while coroutine.resume(c, 2.5) do Yield() end
    end,

    talk = function (self, e)
        local w = MessageWindow.open()

        self.awaits:sleep(0.5); Sound.dynamic("mew_sheep.mp3")
        w:streamText([[メ～、メ～]])
        while w:isRunning() do Yield() end

        if 
            FlagManager.getFlag(FlagName.try_dump_paper) and  
            not FlagManager.getFlag(FlagName.dump_paper_by_eat)
        then
            local s = SelectionWindow.open({[[書類食べて]]})
            while s:isRunning() do Yield() end
            s:close()

            w:streamText("\n"..[[何これ、え、食べるのですか?]])
            while w:isRunning() do Yield() end

            w:streamText("\n"..[[げげ、ぼくはヤギじゃなくて]].."\n"..[[羊さんです..]])
            while w:isRunning() do Yield() end

            local checkedContent = false

            repeat     
                local table = {[[はいどうぞ]]}
                if not checkedContent then
                    table[#table+1]=[[中身を確認]]
                end 
                local s1 = SelectionWindow.open(table)
                while s1:isRunning() do Yield() end
                local index=s1:selectedIndex()
                s1:close()

                if index==1 then
                    w:close()
                    w=MessageWindow.open()

                    w:streamText([[一応書類の中身を確認してみよう]])
                    while w:isRunning() do Yield() end

                    w:streamText("\n"..[[うわ、なんかえっちだ..]])
                    while w:isRunning() do Yield() end

                    w:streamText("\n"..[[見なかったことにしよう]])
                    while w:isRunning() do Yield() end

                    w:close()
                    w=MessageWindow.open()

                    w:streamText([[ぼくは羊さんです..]])
                    while w:isRunning() do Yield() end

                    checkedContent = true
                end
            until index==0

            w:streamText("\n"..[[しょうがないな～、]])
            while w:isRunning() do Yield() end
            w:streamText([[もぐ..もぐ]])
            while w:isRunning() do Yield() end
            w:streamText("\n"..[[まずいです..ぴえん]])
            while w:isRunning() do Yield() end

            FlagManager.setFlag(FlagName.dump_paper_by_eat, true)
        else
            if not FlagManager.getFlag(FlagName.dump_paper_by_eat) then
                w:streamText("\n"..[[草花は人を豊かにすると思います]])
                while w:isRunning() do Yield() end
            else
                w:streamText("\n"..[[ぼくは羊ですよ～]])
                while w:isRunning() do Yield() end
            end
        end

        w:close()
    end
    
}








