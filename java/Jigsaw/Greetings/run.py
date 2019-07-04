mkdir mods/org.astro mods/com.greetings
javac -d mods/org.astro src/org.astro/module-info.java src/org.astro/org/astro/World.java
javac -p mods -d mods/com.greetings src/com.greetings/module-info.java src/com.greetings/com/greetings/Main.java
java -p mods -m com.greetings/com.greetings.Main
