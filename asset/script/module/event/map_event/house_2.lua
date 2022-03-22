MapEventManager:addMapEvents({

    house_2 = function (self, e)
        local m = MessageWindow.open()

        if FlagManager.getFlag(FlagName.friend_with_mouse) then
            m:streamText([[���Ȃ��A�l�Y�~��H�ׂȂ������̂�]])
            while m:isRunning() do Yield() end

            m:streamText("\n"..[[�D�������..]])
            while m:isRunning() do Yield() end
        elseif FlagManager.getFlag(FlagName.obtain_super_dash) then
            m:streamText([[�l�Y�~�A�H�ׂ��̂�]])
            while m:isRunning() do Yield() end
            m:streamText("\n"..[[�A�܂�����]])
            while m:isRunning() do Yield() end
        else
            m:streamText([[����Ɍ������Ă���]].."\n"..[[�l�Y�~���������Ƃ������]])
            while m:isRunning() do Yield() end
    
            m:streamText("\n"..[[�����Ƃ��������Ɍ��܂��Ă��]])
            while m:isRunning() do Yield() end
    
            m:streamText("\n"..[[�����͂Ȃ�����]])
            while m:isRunning() do Yield() end

            FlagManager.setFlag(FlagName.heard_mouse_rumor, true)
        end

        m:close()

    end
})
