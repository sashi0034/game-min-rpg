MapEventManager:addMapEvents({

    house_1 = function (self, e)
        OutLog("Enter house.\n")
        local c = coroutine.create( self.doSleep )
        while coroutine.resume(c, 2.5) do Yield() end
        OutLog("Ok.\n")
    end,

    house_2 = function (self, e)
        local w = MessageWindow.open()

        w:streamText("����͉Ƃł��I\n�ق��ق�")
        while w:isRunning() do Yield() end

        w:streamText("\n2a�s�ڂǂ��Ȃ邩")
        while w:isRunning() do Yield() end

        w:streamText("\n\n�X�N���[�����Ă܂���")
        while w:isRunning() do Yield() end

        w:close()

        w = MessageWindow.open()
        
        w:streamText([[�E�C���h�E�č\�z���Ă݂�]])
        while w:isRunning() do Yield() end

        w:streamText("\n\n\n�������")
        while w:isRunning() do Yield() end

        w:close()
    end,

    
    stones_1 = function (self, e)
        local w = MessageWindow.open()

        w:streamText("�΂��낪�]�����Ă���")

        
        local c = coroutine.create( self.doSleep )

        while w:isRunning() do Yield() end
        w:close()

        w = MessageWindow.open()
        w:streamText("����\n�˂ނ��ł�\n�˂܂�")
        while w:isRunning() do Yield() end

        local c = coroutine.create( self.doSleep )
        while coroutine.resume(c, 2.5) do Yield() end

        w:close()
    end
})


