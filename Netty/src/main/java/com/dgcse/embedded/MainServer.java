package com.dgcse.embedded;

import com.dgcse.embedded.common.CliParser;
import com.dgcse.embedded.common.Constant;
import com.dgcse.embedded.db.Hibernate;
import com.dgcse.embedded.server.NettyBaseServer;
import com.dgcse.embedded.server.SocketChannelInitializer;
import io.netty.handler.logging.LoggingHandler;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.File;


/**
 * Created by chzmo on 2016. 9. 29..
 */
public final class MainServer {
    private static final int port = 7777;
    private static final Logger logger = LoggerFactory.getLogger(Constant.SERVER_LOGGER);

    public static void main(String[] args) throws Exception{
        logger.info("Server Program Initalization");

        CliParser parser = new CliParser();
        parser.parse(args);
        String dbConfPath = parser.getDbConfPath();

        logger.info("Hibernate Configuration Path : {}",dbConfPath);

        File file = new File(dbConfPath);
        if(!file.exists())
            throw new Exception("데이터베이스 설정 파일을 찾을 수 없습니다 : "+dbConfPath);

        Hibernate.init(dbConfPath);

        NettyBaseServer.run(port, new LoggingHandler(),new SocketChannelInitializer());
    }
}
