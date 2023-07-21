package de.igelstudios;

import java.io.*;
import java.net.URL;
import java.nio.file.Files;

public class Main {

    public static void main(String[] args) throws IOException {
        File libs = new File("libs/");
        if(!libs.exists()) if(!libs.mkdir())throw new RuntimeException("Could not create Libs folder");
        File file = new File("dependencies.txt");
        String cont = new String(Files.readAllBytes(file.toPath()));
        cont = cont.replace("\n","");
        cont = cont.replace(String.valueOf((char) 13),"");
        String[] deps = cont.split(";");
        String url = deps[0];
        String os = osID();
        for (int i = 1; i < deps.length; i++) {
            deps[i] = deps[i].replace("$OS",os);
            try(InputStream stream = new URL(url + "/" + deps[i]).openStream()) {
                File jar = new File("libs/" + deps[i]);
                if(jar.exists())continue;
                if(!jar.createNewFile())throw new RuntimeException("Could not create file: " + jar.getAbsolutePath());
                Files.write(jar.toPath(),stream.readAllBytes());
            }catch (FileNotFoundException e){
                throw new RuntimeException("The file " + url + "/" + deps[i] + " could not be found");
            }
        }
    }

    public static String osID(){
        String r = "";
        String osn = System.getProperty("os.name").toLowerCase();
        String osa = System.getProperty("os.arch");
        if(osn.contains("windows")){
            if(!osa.contains("64"))r = "natives-windows-x86";
            else r = osa.startsWith("aarch64") ? "natives-windows-arm64":"natives-windows";
        }
        else if(osn.contains("linux")){
            if(osa.startsWith("arm") || osa.startsWith("aarch64"))r = osa.contains("64") || osa.startsWith("armv8") ? "natives-linux-arm64" : "natives-linux-arm32";
            else r = "natives-linux";
        }
        else if(osn.contains("mac os x") || osn.contains("darwin") || osn.contains("osx")){
            r = osa.startsWith("aarch64") ? "natives-macos-arm64" : "natives-macos";
        }
        return r;
    }
}