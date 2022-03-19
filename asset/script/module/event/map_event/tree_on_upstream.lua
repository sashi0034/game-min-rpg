MapEventManager:addMapEvents({
    tree_on_upstream = function (self, e)
        if (FlagManager.getFlag(FlagName.kill_bee)==true) then
            return
        end
        
        local m = MessageWindow.open()
        
        m:streamText("����A�n�`�̑�������݂�����")
        while m:isRunning() do Yield() end

        local opt = {[[�C�ɂ��Ȃ�]], [[����]]}
        if FlagManager.getFlag(FlagName.have_tree_branch) then
            opt[#opt+1] = [[�؂̎}]]
        end
        if FlagManager.getFlag(FlagName.have_wood_sword) then
            opt[#opt+1] = [[�ؓ�]]
        end

        local s = SelectionWindow.open(opt)
        while s:isRunning() do Yield() end
        local word=s:selectedWord()
        s:close()

        if (word=="�C�ɂ��Ȃ�") then
            m:streamText("\n"..[[��Ȃ���������]].."\n"..[[���Ȃ��������Ƃɂ��悤]])
            while m:isRunning() do Yield() end
        elseif word=="����" then
            m:streamText("\n"..[[�����A]])
            m:animShake(1.0)
            while m:isRunning() do Yield() end

            m:streamText([[����!]])
            m:animShake(1.5)
            while m:isRunning() do Yield() end

            local sleep=coroutine.create(self.doSleep)
            while coroutine.resume(sleep, 1.0) do Yield() end
            m:close()
            
            m = MessageWindow.open()
            m:streamText([[����A�I���񂪏o�Ă���!!]])
            while m:isRunning() do Yield() end

            m:animFlash()
            while m:isRunning() do Yield() end
        elseif word=="�؂̎}" then
            m:streamText("\n"..[[�������Q�b�g�����}�ōU�����Ă݂悤]])
            while m:isRunning() do Yield() end

            m:streamText("\n"..[[�Ƃ��A]])
            while m:isRunning() do Yield() end

            m:streamText([[����!]])
            m:animShake(1.5)
            while m:isRunning() do Yield() end

            local sleep=coroutine.create(self.doSleep)
            while coroutine.resume(sleep, 1.0) do Yield() end
            m:close()
            
            m = MessageWindow.open()
            m:streamText([[����A�I���񂪓{���Ă�!!]])
            while m:isRunning() do Yield() end
        elseif word=="�ؓ�" then
            m:streamText("\n"..[[����������Ă���������ōU�����Ă݂悤]])
            while m:isRunning() do Yield() end

            m:streamText("\n"..[[�Ƃ��A]])
            while m:isRunning() do Yield() end

            m:streamText([[�����!]])
            m:animShake(2.0)
            while m:isRunning() do Yield() end

            local sleep=coroutine.create(self.doSleep)
            while coroutine.resume(sleep, 1.0) do Yield() end
            m:close()
            
            m = MessageWindow.open()
            m:streamText([[�悵�A�I��������������!!]])
            while m:isRunning() do Yield() end

            m:streamText("\n"..[[�͂��݂���ɓ��ꂽ]])
            while m:isRunning() do Yield() end
            FlagManager.setFlag(FlagName.kill_bee, true)
            FlagManager.setFlag(FlagName.obtain_honey, true)
        end

        m:close()
    end,
})
    