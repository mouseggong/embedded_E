package com.dgcse.embedded.db;

import com.dgcse.embedded.entity.SensorPacket;
import org.hibernate.SessionFactory;
import org.hibernate.boot.registry.StandardServiceRegistry;
import org.hibernate.boot.registry.StandardServiceRegistryBuilder;
import org.hibernate.cfg.Configuration;

import java.io.File;

/**
 * Created by LeeHyungRae on 2016. 12. 22..
 */
public class Hibernate {
    private static SessionFactory sessionFactory;
    private static String CONF_PATH = "";
    private static Configuration configuration;
    private static Hibernate instance;

    private Hibernate() throws Exception{
        if(configuration==null&&sessionFactory==null) {
            configuration = new Configuration().configure(new File(CONF_PATH));
            configuration.addAnnotatedClass(SensorPacket.class);
            StandardServiceRegistryBuilder sb = new StandardServiceRegistryBuilder();
            sb.applySettings(configuration.getProperties());
            StandardServiceRegistry standardServiceRegistry = sb.build();
            sessionFactory = configuration.buildSessionFactory(standardServiceRegistry);
        }
    }

    public static void init(String confPath){
        CONF_PATH = confPath;
    }

    public static Hibernate getInstance() throws Exception{
        if(instance==null)
            instance = new Hibernate();
        return instance;
    }

    public SessionFactory getSessionFactory(){
        return sessionFactory;
    }

    public void shutdown(){
        sessionFactory.close();
    }
}
