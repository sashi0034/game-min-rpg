MapEventManager:addMapEvents({

    tree_on_upstream = function (self, e)

        
        local m = MessageWindow.open()
        
        m:streamText("おや、ハチの巣があるみたいだ")
        while m:isRunning() do Yield() end

        m:streamText("\n"..[[叩く]])
        while m:isRunning() do Yield() end

        m:close()
    end,
})
    