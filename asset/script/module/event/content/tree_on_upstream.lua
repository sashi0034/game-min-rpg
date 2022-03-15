MapEventManager:addMapEvents({

    tree_on_upstream = function (self, e)

        
        local m = MessageWindow.open()
        
        m:streamText("‚¨‚âAƒnƒ`‚Ì‘ƒ‚ª‚ ‚é‚İ‚½‚¢‚¾")
        while m:isRunning() do Yield() end

        m:streamText("\n"..[[’@‚­]])
        while m:isRunning() do Yield() end

        m:close()
    end,
})
    