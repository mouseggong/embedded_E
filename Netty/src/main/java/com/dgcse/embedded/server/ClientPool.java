package com.dgcse.embedded.server;

import io.netty.channel.ChannelHandlerContext;

import java.util.HashMap;

/**
 * Created by LeeHyungRae on 2016. 12. 22..
 */
public class ClientPool {
    private static ClientPool instance;
    private HashMap<Integer,ChannelHandlerContext> hashMap;//92,83

    private ClientPool(){
        hashMap = new HashMap<>();
    }

    public static ClientPool getInstance(){
        if(instance==null)
            instance = new ClientPool();
        return instance;
    }

    public ChannelHandlerContext getClient(int nodeId){
        return hashMap.get(nodeId);
    }

    public void putClient(int nodeId, ChannelHandlerContext ctx){
        hashMap.remove(nodeId);
        hashMap.put(nodeId,ctx);
    }
}
