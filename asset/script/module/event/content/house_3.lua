MapEventManager:addMapEvents({
    house_3 = function (self, e)
        local m = MessageWindow.open()

        m:streamText([[����ɂ��́A���͕��퉮����ł�]])
        while m:isRunning() do Yield() end

        if (FlagManager.getFlag(FlagName.have_tree_branch)) and (FlagManager.getFlag(FlagName.have_wood_sword)==false) then
            m:streamText("\n"..[[����A�f���炵���؂̎}��]].."\n"..[[�������̂悤�ł���]])
            while m:isRunning() do Yield() end

            m:streamText("\n"..[[���ʂɖؓ����쐬���Ă����܂��傤��]])
            while m:isRunning() do Yield() end

            local s = SelectionWindow.open({[[����]], [[����]]})
            while s:isRunning() do Yield() end
            local index=s:selectedIndex()
            s:close()

            if (index==0) then
                local sleep
                m:streamText("\n"..[[��������A��������]])
                while m:isRunning() do Yield() end
                sleep=coroutine.create(self.doSleep)
                while coroutine.resume(sleep, 0.5) do Yield() end

                m:streamText("\n"..[[�Ƃ��[�A�����[]])
                while m:isRunning() do Yield() end
                sleep=coroutine.create(self.doSleep)
                while coroutine.resume(sleep, 0.5) do Yield() end

                m:streamText("\n"..[[�΂����A�΂���]])
                while m:isRunning() do Yield() end
                sleep=coroutine.create(self.doSleep)
                while coroutine.resume(sleep, 0.5) do Yield() end

                m:streamText("\n"..[[���҂������܂���]].."\n"..[[�؂̌������������̂ō����グ�܂�]])
                while m:isRunning() do Yield() end

                FlagManager.setFlag(FlagName.have_tree_branch, false)
                FlagManager.setFlag(FlagName.have_wood_sword, true)
            elseif index==1 then
                m:streamText("\n"..[[���`]].."\n"..[[�����[!]])
                while m:isRunning() do Yield() end
            end

        else
            m:streamText("\n"..[[��ϐ\���󂲂����܂���...]])
            while m:isRunning() do Yield() end

            local sleep=coroutine.create(self.doSleep)
            while coroutine.resume(sleep, 0.5) do Yield() end

            m:streamText("\n"..[[���X�ł͌�p�̂Ȃ�����]].."\n"..[[���X�����f�肵�Ă��܂�]])
            while m:isRunning() do Yield() end

            m:streamText("\n"..[[�܂��̂��z�������҂����Ă���܂�]])
            while m:isRunning() do Yield() end    
        end

        m:close()
    end})