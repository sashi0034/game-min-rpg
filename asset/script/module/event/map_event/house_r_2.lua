MapEventManager:addMapEvents({

    house_r_2 = function (self, e)
        local m = MessageWindow.open()

        if not FlagManager.getFlag(FlagName.dump_paper_by_eat) then
            if not FlagManager.getFlag(FlagName.try_dump_paper) then
                m:streamText([[���̏��ނ��ǂ����Ɏ̂Ă�]].."\n"..[[���Ăق�����]])
                while m:isRunning() do Yield() end
        
                local s = SelectionWindow.open({[[�͂�]], [[������]]})
                while s:isRunning() do Yield() end
                local index=s:selectedIndex()
                s:close()
        
                if index==0 then
                    m:streamText("\n"..[[���ނˁA���g�͌�����_������]])
                    while m:isRunning() do Yield() end
        
                    FlagManager.setFlag(FlagName.try_dump_paper, true)
                else
                    m:streamText("\n"..[[�������A�܂�������]])
                    while m:isRunning() do Yield() end
                end
            else
                m:streamText([[�o������N���ɐH�ׂ�]].."\n"..[[������Ă��Ăق�����]])
                while m:isRunning() do Yield() end
            end
        else
            m:streamText([[���ނ��̂ĂĂ��Ă��ꂽ�̂���]].."\n"..[[���肪�Ƃ��ˁ`]])
            while m:isRunning() do Yield() end
        end

        m:close()
    end
})
