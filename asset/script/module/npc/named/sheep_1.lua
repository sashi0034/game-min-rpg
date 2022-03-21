
-- todo: è†‚ğ”j‚Á‚Ä‚à‚ç‚¤‹@”\‚ğ‚½‚¹‚é



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

        w:streamText([[ƒ`Aƒ`]])
        while w:isRunning() do Yield() end

        if 
            FlagManager.getFlag(FlagName.try_dump_paper) and  
            not FlagManager.getFlag(FlagName.dump_paper_by_eat)
        then
            local s = SelectionWindow.open({[[‚±‚êH‚×‚Ä]]})
            while s:isRunning() do Yield() end
            s:close()

            w:streamText("\n"..[[‰½‚±‚êA‰½‚©‚Ì‘—Ş‚Å‚·‚©?]])
            while w:isRunning() do Yield() end

            w:streamText("\n"..[[‚°‚°A‚Ú‚­‚Íƒ„ƒM‚¶‚á‚È‚­‚Ä]].."\n"..[[—r‚³‚ñ‚Å‚·..]])
            while w:isRunning() do Yield() end

            local checkedContent = false

            repeat     
                local table = {[[‚Í‚¢‚Ç‚¤‚¼]]}
                if not checkedContent then
                    table[#table+1]=[[’†g‚ğŠm”F]]
                end 
                local s1 = SelectionWindow.open(table)
                while s1:isRunning() do Yield() end
                local index=s1:selectedIndex()
                s1:close()

                if index==1 then
                    w:close()
                    w=MessageWindow.open()

                    w:streamText([[ˆê‰’†g‚ğŠm”F‚µ‚Ä‚İ‚æ‚¤]])
                    while w:isRunning() do Yield() end

                    w:streamText("\n"..[[‚¤‚íA‚È‚ñ‚©‚¦‚Á‚¿‚¾..]])
                    while w:isRunning() do Yield() end

                    w:streamText("\n"..[[Œ©‚È‚©‚Á‚½‚±‚Æ‚É‚µ‚æ‚¤]])
                    while w:isRunning() do Yield() end

                    w:close()
                    w=MessageWindow.open()

                    w:streamText([[‚Ú‚­‚Í—r‚³‚ñ‚Å‚·..]])
                    while w:isRunning() do Yield() end

                    checkedContent = true
                end
            until index==0

            w:streamText("\n"..[[‚µ‚å‚¤‚ª‚È‚¢‚È`A]])
            while w:isRunning() do Yield() end
            w:streamText([[‚à‚®..‚à‚®]])
            while w:isRunning() do Yield() end
            w:streamText("\n"..[[‚Ü‚¸‚¢‚Å‚·..‚Ò‚¦‚ñ]])
            while w:isRunning() do Yield() end

            FlagManager.setFlag(FlagName.dump_paper_by_eat, true)
        else
            if not FlagManager.getFlag(FlagName.dump_paper_by_eat) then
                w:streamText("\n"..[[‘‰Ô‚ÉˆÍ‚Ü‚ê‚Ä‘–‚é‚Ì‚Í‚¢‚¢‚Å‚·‚Ë`]])
                while w:isRunning() do Yield() end
            else
                w:streamText("\n"..[[‚Ú‚­‚Í—r‚Å‚·‚æ`]])
                while w:isRunning() do Yield() end
            end
        end

        w:close()
    end
    
}








