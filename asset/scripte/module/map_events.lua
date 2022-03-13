MapEventManager:addMapEvents({

    house_1 = function (self, e)
        OutLog("Enter house.\n")
        local c = coroutine.create( self.doSleep )
        while coroutine.resume(c, 2.5) do Yield() end
        OutLog("Ok.\n")
    end,

    house_2 = function (self, e)
        local m = MessageWindow.open()

        m:streamText("���͂悤�������܂�")
        while m:isRunning() do Yield() end

        local s = SelectionWindow.open({"����ɂ���", "���͂悤", "����", "������", "���肪�Ƃ�"})
        while s:isRunning() do Yield() end
        local index=s:selectedIndex()
        s:close()

        if index==0 then
            m:streamText("\n"..[[����ɂ��͂���ɂ���]])
            while m:isRunning() do Yield() end
        elseif index==1 then
            m:streamText("\n"..[[���͂悤�������܂��A���͂悤�������܂��A���͂悤�������܂�]])
            while m:isRunning() do Yield() end
        elseif index==2 then
            m:streamText("\n"..[[�����ł���]])
            while m:isRunning() do Yield() end
        elseif index==3 then
            m:streamText("\n"..[[�����������炱��]].."\n"..[[�Ȃɂ���??]])
            while m:isRunning() do Yield() end
        elseif index==4 then
            m:streamText("\n"..[[���ӂ̋C�����������܂��傤]])
            while m:isRunning() do Yield() end
        end

        m:close()
    end,

    
    stones_1 = function (self, e)
        local w = MessageWindow.open()

        w:streamText("�΂��낪�]�����Ă���")
        while w:isRunning() do Yield() end
        
        local s = SelectionWindow.open({"�͂�", "������"})
        while s:isRunning() do Yield() end
        s:close()

        Yield()
        
        w:close()

        w = MessageWindow.open()
        w:streamText("����\n�˂ނ��ł�\n�˂܂�")
        while w:isRunning() do Yield() end

        local c = coroutine.create( self.doSleep )
        while coroutine.resume(c, 2.5) do Yield() end

        w:close()
    end,

    tree_1 = function (self, e)
        local w = MessageWindow.open()

        w:streamText("���ʂ̖؂�����")
        while w:isRunning() do Yield() end
   
        w:close()
    end
})


