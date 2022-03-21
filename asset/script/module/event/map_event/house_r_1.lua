MapEventManager:addMapEvents({

    house_r_1 = function (self, e)
        local m = MessageWindow.open()

        if not FlagManager.getFlag(FlagName.gave_herbs) then
            m:streamText([[���ق�A���ׂ��Ђ��Ă��܂���]].."\n"..[[���܂����˂�]])
            while m:isRunning() do Yield() end

            local table = {[[�����?]], [[�΂��΂�]]}

            if 
                FlagManager.getFlag(FlagName.obtain_herbs_near_bush) and
                FlagManager.getFlag(FlagName.obtain_herbs_near_castle) and
                FlagManager.getFlag(FlagName.obtain_herbs_under_tree)
            then
                table[#table+1] = [[���򂠂���]]
            end

            local s = SelectionWindow.open(table)
            while s:isRunning() do Yield() end
            local word=s:selectedWord()
            s:close()

            local bye = function ()
                m:streamText("\n"..[[�����A�����A��̂���]])
                while m:isRunning() do Yield() end
                m:streamText("\n"..[[�܂����Ă����Ƃ��ꂵ����]])
                while m:isRunning() do Yield() end
            end
            if word==[[�����?]] then
                m:streamText("\n"..[[��? �����Ă��܂����˂��A]].."\n"..[[�ǂ����ɑ��݂���Ƃ͕�������..]])
                while m:isRunning() do Yield() end

                local s1 = SelectionWindow.open({[[�ǂ�?]], [[�΂��΂�]]})
                while s1:isRunning() do Yield() end
                local index1=s1:selectedIndex()
                s1:close()

                if index1==0 then
                    m:streamText("\n"..[[�ꏊ�˂�..]])
                    while m:isRunning() do Yield() end

                    m:streamText("\n"..[[��̐X��3���񂾖΂݂̂̊ԁA]].."\n"..[[��̐X�̍ł����̖؉A�A]])
                    while m:isRunning() do Yield() end

                    m:streamText("\n"..[[���Ƃ͂���̓쐼�̖΂�]])
                    while m:isRunning() do Yield() end

                    m:streamText("\n"..[[���̂�����Ŗ򂪍̂����ĕ����˂�]])
                    while m:isRunning() do Yield() end

                    local s2 = SelectionWindow.open({[[�C����]], [[�΂��΂�]]})
                    while s2:isRunning() do Yield() end
                    local index2=s2:selectedIndex()
                    s2:close()

                    if index2==0 then
                        m:streamText("\n"..[[����A�Ƃ��Ă��Ă����̂���]].."\n"..[[���������˂�]])
                        while m:isRunning() do Yield() end
                    elseif index2==1 then
                        bye()
                    end
                elseif index1==1 then
                    bye()
                end
            elseif word==[[���򂠂���]] then
                m:streamText("\n"..[[����A����Ƃ��Ă��Ă��ꂽ�̂���]])
                while m:isRunning() do Yield() end
                m:streamText("\n"..[[�₳�����q���ˁA���肪�Ƃ�]])
                while m:isRunning() do Yield() end

                FlagManager.setFlag(FlagName.gave_herbs, true)
            else
                bye()
            end
        else
            m:streamText([[���݂̂��A�ŏ���������]].."\n"..[[�{���ɂ��肪�Ƃ���]])
            while m:isRunning() do Yield() end
        end

        m:close()
    end
})
