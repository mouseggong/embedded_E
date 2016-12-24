package com.dgcse.embedded.server;

import com.dgcse.embedded.handler.MessageAggregator;
import com.dgcse.embedded.handler.SensorPacketHandler;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelPipeline;
import io.netty.channel.socket.SocketChannel;

/**
 * Created by LeeHyungRae on 2016. 12. 22..
 */
public class SocketChannelInitializer extends ChannelInitializer<SocketChannel>{

    private ChannelPipeline channelPipeline;

    @Override
    protected void initChannel(SocketChannel ch) throws Exception {
        channelPipeline = ch.pipeline();
        channelPipeline.addLast(new MessageAggregator());
        channelPipeline.addLast(new SensorPacketHandler());
    }
}
