

mouse_fast = {

    new = function()
        
        local self = Instantiate(mouse_fast, MouseLuaData)
        
        self.events = {
            move = nil,
        }
        self.vel = 80
        self.frameInterval = 60

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
        e1 = MapEventManager.getUnique("mouse_fast_move_1")
        e2 = MapEventManager.getUnique("mouse_fast_move_2")
        e3 = MapEventManager.getUnique("mouse_fast_move_3")
        e4 = MapEventManager.getUnique("mouse_fast_move_4")

        while self.doMove(e1.x, e1.y) do Yield() end
        while self.doMove(e2.x, e2.y) do Yield() end
        while self.doMove(e3.x, e3.y) do Yield() end
        while self.doMove(e4.x, e4.y) do Yield() end

        self.setDeath(true)
    end,

    talk = function (self, e)
        local m = MessageWindow.open()

        if 
            FlagManager.getFlag(FlagName.obtain_super_dash) or
            FlagManager.getFlag(FlagName.friend_with_mouse) 
        then
            m:streamText([[�ځA�l�͈����Ȃ�����!]])
            while m:isRunning() do Yield() end

            m:close()
            return
        end


        m:streamText([[�ځA�l��H�ׂȂ��ł������!]])
        while m:isRunning() do Yield() end

        -- m:streamText("\n"..[[�������A�N�ɂ��˂��݂̉��������]].."\n"..[[���������]])
        -- while m:isRunning() do Yield() end

        local s = SelectionWindow.open({[[������]], [[�H�ׂ�]]})
        while s:isRunning() do Yield() end
        local index=s:selectedIndex()
        s:close()

        if (index==0) then
            local sleep

            m:streamText("\n"..[[���肪�Ƃ�����! ]])
            while m:isRunning() do Yield() end

            FlagManager.setFlag(FlagName.friend_with_mouse, true)
        elseif index==1 then
            local sleep

            m:streamText("\n"..[[�ЁA�Ђǂ�����! ]])
            while m:isRunning() do Yield() end

            sleep=coroutine.create(self.doSleep)
            while coroutine.resume(sleep, 0.5) do Yield() end

            m:close()
            m = MessageWindow.open()

            m:streamText([[�ނ���ނ���A]])
            while m:isRunning() do Yield() end
            sleep=coroutine.create(self.doSleep)
            while coroutine.resume(sleep, 0.5) do Yield() end

            m:streamText([[��������]])
            while m:isRunning() do Yield() end
            sleep=coroutine.create(self.doSleep)
            while coroutine.resume(sleep, 0.5) do Yield() end

            m:streamText("\n"..[[Shift�̃_�b�V���������Ƒ����Ȃ���!]])
            while m:isRunning() do Yield() end

            FlagManager.setFlag(FlagName.obtain_super_dash, true)
            self.fadeAndDie()
        end

        m:close()
    end
    
}








