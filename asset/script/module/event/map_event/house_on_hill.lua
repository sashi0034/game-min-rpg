MapEventManager:addMapEvents({

    house_on_hill = function (self, e)
        local m = MessageWindow.open()

        if FlagManager.getFlag(FlagName.obtain_solt) then
            m:streamText([[�͂��݂���������]])
            while m:isRunning() do Yield() end

            m:streamText("\n"..[[�؂�؂�A�؂�؂�]])
            while m:isRunning() do Yield() end

            m:close()
            return
        end

        m:streamText([[�͂��݂��H�ׂ�����]])
        while m:isRunning() do Yield() end

        if FlagManager.getFlag(FlagName.obtain_honey) then
            m:streamText("\n"..[[���񂭂�.. ]])
            while m:isRunning() do Yield() end

            m:streamText([[���݂͂͂��݂�]].."\n"..[[�����Ă���񂶂�Ȃ�����?]])
            while m:isRunning() do Yield() end

            local index=0
            local refuseCount=0

            repeat
                local s = SelectionWindow.open({[[������]], [[�����Ȃ�]]})
                while s:isRunning() do Yield() end
                index=s:selectedIndex()
                s:close()

                if index==1 then
                    m:streamText("\n"..[[�₾�A���肢!]])
                    while m:isRunning() do Yield() end
                    refuseCount=refuseCount+1
                end
            until index==0

            if refuseCount==0 then
                m:streamText("\n"..[[���肪�Ƃ�!]])
                while m:isRunning() do Yield() end
            else
                m:streamText("\n"..[[�����������Ă��߂��..]])
                while m:isRunning() do Yield() end
            end

            m:streamText("\n"..[[����ɋ߂��łƂꂽ�≖��]].."\n"..[[�������]])
            while m:isRunning() do Yield() end

            FlagManager.setFlag(FlagName.obtain_solt, true)
        end

        m:close()
    end
})
