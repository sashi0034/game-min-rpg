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
    end
})
