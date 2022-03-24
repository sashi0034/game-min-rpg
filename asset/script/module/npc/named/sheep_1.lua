

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
        w:streamText([[���`�A���`]])
        while w:isRunning() do Yield() end

        if 
            FlagManager.getFlag(FlagName.try_dump_paper) and  
            not FlagManager.getFlag(FlagName.dump_paper_by_eat)
        then
            local s = SelectionWindow.open({[[���ސH�ׂ�]]})
            while s:isRunning() do Yield() end
            s:close()

            w:streamText("\n"..[[������A���A�H�ׂ�̂ł���?]])
            while w:isRunning() do Yield() end

            w:streamText("\n"..[[�����A�ڂ��̓��M����Ȃ���]].."\n"..[[�r����ł�..]])
            while w:isRunning() do Yield() end

            local checkedContent = false

            repeat     
                local table = {[[�͂��ǂ���]]}
                if not checkedContent then
                    table[#table+1]=[[���g���m�F]]
                end 
                local s1 = SelectionWindow.open(table)
                while s1:isRunning() do Yield() end
                local index=s1:selectedIndex()
                s1:close()

                if index==1 then
                    w:close()
                    w=MessageWindow.open()

                    w:streamText([[�ꉞ���ނ̒��g���m�F���Ă݂悤]])
                    while w:isRunning() do Yield() end

                    w:streamText("\n"..[[����A�Ȃ񂩂�������..]])
                    while w:isRunning() do Yield() end

                    w:streamText("\n"..[[���Ȃ��������Ƃɂ��悤]])
                    while w:isRunning() do Yield() end

                    w:close()
                    w=MessageWindow.open()

                    w:streamText([[�ڂ��͗r����ł�..]])
                    while w:isRunning() do Yield() end

                    checkedContent = true
                end
            until index==0

            w:streamText("\n"..[[���傤���Ȃ��ȁ`�A]])
            while w:isRunning() do Yield() end
            w:streamText([[����..����]])
            while w:isRunning() do Yield() end
            w:streamText("\n"..[[�܂����ł�..�҂���]])
            while w:isRunning() do Yield() end

            FlagManager.setFlag(FlagName.dump_paper_by_eat, true)
        else
            if not FlagManager.getFlag(FlagName.dump_paper_by_eat) then
                w:streamText("\n"..[[���Ԃ͐l��L���ɂ���Ǝv���܂�]])
                while w:isRunning() do Yield() end
            else
                w:streamText("\n"..[[�ڂ��͗r�ł���`]])
                while w:isRunning() do Yield() end
            end
        end

        w:close()
    end
    
}








