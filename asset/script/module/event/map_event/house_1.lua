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
            if not FlagManager.getFlag(FlagName.broke_tree_branch) then
                m:streamText("\n"..[[�ŋߏ�̖k�̖؂�����Ă���̂�]])
                while m:isRunning() do Yield() end
                m:streamText("\n"..[[Space�L�[�ł������炵�Ă͂�����]])
                while m:isRunning() do Yield() end
            elseif (not FlagManager.getFlag(FlagName.kill_bee)) then
                m:streamText("\n"..[[���̖؂Ƀn�`�̑����ł����ƕ����Ă���]])
                while m:isRunning() do Yield() end
                m:streamText("\n"..[[��Ȃ�����Space�L�[�Œ��ׂĂ͂�����]])
                while m:isRunning() do Yield() end
            elseif not FlagManager.getFlag(FlagName.kill_slime) then
                m:streamText("\n"..[[�F���犴�ӂ���邱�Ƃ����Ȃ���]]..[[��������΋F�肪�͂�����낤]])
                while m:isRunning() do Yield() end
            else
                m:streamText("\n"..[[�킵���猾���邱�Ƃ͂����Ȃ�]].."\n"..[[������̂��Ⴜ]])
                while m:isRunning() do Yield() end
            end

        elseif index==1 then
            m:streamText("\n"..[[�N�͉������ɗ����̂���]])
            while m:isRunning() do Yield() end
        end

        m:close()
    end
})
