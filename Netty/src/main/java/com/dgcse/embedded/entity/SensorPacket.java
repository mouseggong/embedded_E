package com.dgcse.embedded.entity;

import javax.persistence.*;
import java.util.Calendar;
import java.util.Date;

/**
 * Created by LeeHyungRae on 2016. 12. 22..
 */
@Entity(name="sensor_data")
@Table(name="sensor_data")
public class SensorPacket {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private int id;
    private int node_id;
    private int light;
    private int temp;
    private int heartbeat;
    private int menu;
    private Date date;

    public SensorPacket(int node_id, int light, int temp, int heartbeat, int menu) {
        this.node_id = node_id;
        this.light = light;
        this.temp = temp;
        this.heartbeat = heartbeat;
        this.menu = menu;
        this.date = Calendar.getInstance().getTime();
    }

    public int getNode_id() {
        return node_id;
    }

    public void setNode_id(int node_id) {
        this.node_id = node_id;
    }

    public int getLight() {
        return light;
    }

    public void setLight(int light) {
        this.light = light;
    }

    public int getTemp() {
        return temp;
    }

    public void setTemp(int temp) {
        this.temp = temp;
    }

    public int getHeartbeat() {
        return heartbeat;
    }

    public void setHeartbeat(int heartbeat) {
        this.heartbeat = heartbeat;
    }

    public int getMenu() {
        return menu;
    }

    public void setMenu(int menu) {
        this.menu = menu;
    }

    public Date getDate() {
        return date;
    }

    public void setDate(Date date) {
        this.date = date;
    }

    @Override
    public String toString() {
        String msg = String.format("[%d] light:%d, temp:%d, heartbeat:%d, menu:%d", node_id,light,temp,heartbeat,menu);
        return msg;
    }
}
