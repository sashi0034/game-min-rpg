MapEventManager:addMapEvents({

    house_1 = function (self, e)
        local m = MessageWindow.open()

        m:streamText([[�����p����?]])
        while m:isRunning() do Yield() end

        local s = SelectionWindow.open({[[������]], [[������]]})
        while s:isRunning() do Yield() end
        local index=s:selectedIndex()
        s:close()

        if (index==0) then
            m:streamText("\n"..[[���̖؂Ƀn�`�̑����ł����ƕ����Ă���]])
            while m:isRunning() do Yield() end
            m:streamText("\n"..[[��Ȃ�����Space�L�[�Œ��ׂĂ͂�����]])
            while m:isRunning() do Yield() end
        elseif index==1 then
            m:streamText("\n"..[[�N�͉������ɗ����̂���]])
            while m:isRunning() do Yield() end
        end

        m:close()
    end,

    house_2 = function (self, e)

    end,


    house_3 = function (self, e)
        local m = MessageWindow.open()
        
        m:streamText([[����ɂ��́A���͕��퉮����ł�]])
        while m:isRunning() do Yield() end

        OutLog((FlagName.obtain_tree_branch).."")

        if (FlagManager.getFlag(FlagName.obtain_tree_branch)) and (not FlagManager.getFlag(FlagName.create_wood_sword)) then
            m:streamText("\n"..[[����A�f���炵���؂̎}��]].."\n"..[[�������̂悤�ł���]])
            while m:isRunning() do Yield() end

            m:streamText("\n"..[[���ʂɖؓ����쐬���Ă����܂��傤]])
            while m:isRunning() do Yield() end

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

            FlagManager.getFlag(FlagName.obtain_tree_branch, false)
            FlagManager.getFlag(FlagName.create_wood_sword, true)
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
    end,



    tree_1 = function (self, e)
        if FlagManager.getFlag(FlagName.obtain_tree_branch)==true then
            return
        end
        
        local m = MessageWindow.open()
        

        m:streamText("���C�Ȗ؂�����Ă��")
        while m:isRunning() do Yield() end

        m:streamText("\n"..[[����A�}�����낢��]])
        while m:isRunning() do Yield() end

        local s = SelectionWindow.open({[[�����Ƃ���]], [[�܂����Ⴄ]]})
        while s:isRunning() do Yield() end
        local index=s:selectedIndex()
        s:close()

        if (index==0) then
            m:streamText("\n"..[[���̖؂������Ă��邩���]].."\n"..[[�����Ƃ��Ă�����]])
            while m:isRunning() do Yield() end
        elseif index==1 then
            m:streamText("\n"..[[�����Ɏg������������Ƃ��Ă�����]])
            while m:isRunning() do Yield() end
            FlagManager.setFlag(FlagName.obtain_tree_branch, true)
        end

        m:close()
    end,

    tree_on_upstream = function (self, e)
        if FlagManager.getFlag(FlagName.obtain_tree_branch)==true or FlagManager.getFlag(FlagName.create_wood_sword)==true then
            return
        end
        
        local m = MessageWindow.open()
        

        m:streamText("���C�Ȗ؂�����Ă��")
        while m:isRunning() do Yield() end

        m:streamText("\n"..[[����A�}�����낢��]])
        while m:isRunning() do Yield() end


    end
})


