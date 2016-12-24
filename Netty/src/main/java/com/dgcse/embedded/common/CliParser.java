package com.dgcse.embedded.common;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.DefaultParser;
import org.apache.commons.cli.Options;

/**
 * Created by LeeHyungRae on 2016. 12. 22..
 */
public class CliParser {

    private CommandLineParser commandLineParser;
    private CommandLine cmdLine;
    private Options options;

    public CliParser(){
        commandLineParser = new DefaultParser();
        init();
    }

    private void init(){
        options = new Options();
        options.addOption("d","db",true,"Hibernate.cfg.xml File Location");
    }

    public void parse(String[] args) throws Exception{
        cmdLine = commandLineParser.parse(options,args);
        if(!cmdLine.hasOption("d"))
            throw new Exception("Database Configuration File 옵션이 설정되지 않았습니다.");
    }

    public String getDbConfPath(){
        return cmdLine.getOptionValue("d");
    }

}
