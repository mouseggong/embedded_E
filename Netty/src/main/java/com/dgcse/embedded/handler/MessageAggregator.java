package com.dgcse.embedded.handler;

import com.dgcse.embedded.common.Constant;
import com.dgcse.embedded.entity.SensorPacket;
import com.dgcse.embedded.server.ClientPool;
import io.netty.buffer.ByteBuf;
import io.netty.channel.ChannelHandlerContext;
import io.netty.handler.codec.ByteToMessageDecoder;
import io.netty.util.concurrent.Future;
import io.netty.util.concurrent.GenericFutureListener;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.List;

/**
 * byte 형식으로 수신한 정보를 Message 형태로 변경한다.
 */
public class MessageAggregator extends ByteToMessageDecoder {

    private static final int MSG_LEN = 17;
    private static final Logger logger = LoggerFactory.getLogger(Constant.SERVER_LOGGER);

    private static final int[] RECV_SIG_BYTE = {0xA0, 0x0A, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x0A,0xA0};

    private byte[] recvByteArr;

    private boolean isStart = false;

    public MessageAggregator() {
        super();
        recvByteArr = new byte[MSG_LEN];
    }
//ctx.pipeline.writeandflush();
    @Override
    public void channelActive(ChannelHandlerContext ctx) throws Exception {
        String ip = ctx.channel().remoteAddress().toString();

        logger.info("Client Connected : {}",ip);
        ctx.writeAndFlush(RECV_SIG_BYTE);

        if(ip.contains("92")){
            ClientPool.getInstance().putClient(92,ctx);
        }
        else{
            ClientPool.getInstance().putClient(83,ctx);
        }


    }

    @Override
    protected void decode(ChannelHandlerContext ctx, ByteBuf in, List<Object> out) throws Exception {
        //첫번째 1바이트는 에러 코드이므로 제거한다
        if(!isStart&&in.readableBytes()>0){
            //Discard First Packet
            in.readBytes(1).release();
            isStart = true;
        }
        else if(in.readableBytes()>=MSG_LEN){
            ByteBuf byteBuf = in.readBytes(MSG_LEN);
            byteBuf.readBytes(recvByteArr).release();
            int[] decodedArr = decodeUnsigned(recvByteArr);
            SensorPacket packet = decodeSensorPacket(decodedArr);
            logger.debug(packet.toString());

            if(packet.getNode_id()==0){
                logger.info("Node ID is 0.");
            }
            else{
                ClientPool.getInstance().getClient(83).channel().writeAndFlush(RECV_SIG_BYTE);
            }
        }
    }

    private int[] decodeUnsigned(byte[] arr){
        int[] result = new int[arr.length];

        for(int i = 0;i<arr.length;i++){
            if(arr[i]>=0)
                result[i] = arr[i];
            else
                result[i] = arr[i]+256;
        }
        return result;
    }

    private SensorPacket decodeSensorPacket(int[] arr){
        int id = arr[3];
        int light = (arr[4]<<8)|(arr[5]);
        int temp = arr[6];
        int heartbeat = arr[7];
        int menu = arr[8];
        int sum=0;
        int csc=0;
        int[] sendingmsg = {0xA0, 0x0A, 0x00, 0x00, 0x01, 0x00, 0x00, 255, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x0A,0xA0};
        int i=0;
        int emergency = 0;
        if(ClientPool.getInstance().getClient(83).name().length()>0) {
            if (arr[7] != 0) {
                heartbeat = arr[7];
            }

            for (i = 2; i < 14; i++) {
                sum += arr[i];
            }

            if (sum == csc) {
                //printf("[%03d] 밝기:%d, 온도:%d, 심박(BPM):%d, menu:%d\n", id, light, temp, heartbeat, menu);
            } else {
                //printf("[%03d] Checksum error\n", id);
            }
            if (menu > 0) {
                sendingmsg[5] = 255;
                sendingmsg[6] = 0;
                switch (menu) {
                    case 1:
                        //printf("help\n");
                        break;
                    case 2:
                        //printf("Restroom\n");
                        break;
                    case 3:
                        //printf("Thirsty\n");
                        break;
                    case 4:
                        //printf("Hungry\n");
                        break;
                    case 5:
                        //printf("Light On\n");
                        break;
                    case 6:
                        //printf("Light Off\n");
                        break;
                }
            } else {
                //printf("No problem\n");
            }
            //if(temp<25 || temp>40){

            if (temp > 40 || temp < 20) {
                //printf("온도 많이 이상 : %d\n", temp);
                emergency = 1;
            } else if (temp > 37) {
                //printf("온도 조금 이상 : %d\n", temp);
                sendingmsg[4] = 255;
                sendingmsg[5] = 255;
                sendingmsg[6] = 0;
            }

            if (heartbeat != 0 && (heartbeat < 50 || heartbeat > 300)) {
                //printf("심박 이상: %d\n", heartbeat);
                emergency = 1;
            }

            if (emergency == 1) {
                sendingmsg[4] = 255;
                sendingmsg[5] = 0;
                sendingmsg[6] = 0;
                sendingmsg[7] = 1;
            }
            for (i = 0; i < 17; i++) {
                //printf("%d ",(int)sendingmsg[i]);
            }

            ClientPool.getInstance().getClient(83).channel().writeAndFlush(sendingmsg);
        }
        return new SensorPacket(id,light,temp,heartbeat,menu);
    }

    @Override
    public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) throws Exception {
        logger.info("Exception Occured In Message Aggregator",cause);
        ctx.channel().closeFuture().addListener(new GenericFutureListener<Future<? super Void>>() {
            @Override
            public void operationComplete(Future<? super Void> future) throws Exception {
                if(future.isSuccess())
                    logger.info("Client Close Successfully");
                else
                    logger.info("Client Close Failed");
            }
        });
    }

    @Override
    public void channelUnregistered(ChannelHandlerContext ctx) throws Exception {
        logger.info("Client Connection Closed : {}",ctx.channel().remoteAddress().toString());
    }
}
