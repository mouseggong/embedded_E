package com.dgcse.embedded.handler;

import com.dgcse.embedded.common.Constant;
import com.dgcse.embedded.db.Hibernate;
import com.dgcse.embedded.entity.SensorPacket;
import io.netty.channel.ChannelHandler;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.SimpleChannelInboundHandler;
import org.hibernate.Session;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Created by LeeHyungRae on 2016. 12. 22..
 */
@ChannelHandler.Sharable
public class SensorPacketHandler extends SimpleChannelInboundHandler<SensorPacket>{

    private static final Logger logger = LoggerFactory.getLogger(Constant.SERVER_LOGGER);

    @Override
    protected void channelRead0(ChannelHandlerContext ctx, SensorPacket msg) throws Exception {
        Session session = Hibernate.getInstance().getSessionFactory().getCurrentSession();
        session.beginTransaction();
        session.save(msg);
        session.getTransaction().commit();
        logger.debug("{} saved",msg);
        session.close();

    }

}
