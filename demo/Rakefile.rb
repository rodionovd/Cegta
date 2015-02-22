task :default => [:execute, :cleanup]

# Configuration
SOURCES = "demo.c"
SHARED_CCFLAGS = "-std=c99"

specs = [
	{ :compiler => "clang", :arch => "x86_64", :ccflags => "-O0" },
]

desc "Build test cases using clang and gcc compilers for i386 and x86_64 architectures"
task :build do
	system("mkdir build") if (!File.exists?(Dir.getwd + "/build"))
	specs.each {|entry|
		system("#{entry[:compiler]} -arch #{entry[:arch]} #{SHARED_CCFLAGS} #{entry[:ccflags]} -o ./build/#{filename_for_spec(entry)} #{SOURCES}")
		if $?.exitstatus == 0 then
			entry[:result] = true;
		else
			entry[:result] = false;
			puts "Fail to compile test (#{outfile})".red
		end
	}
end

desc "Run test cases"
task :execute => [:build] do
	specs.each {|entry|
		if entry[:result] != false then
			system("./build/#{filename_for_spec(entry)}")
			entry[:result] = false if $?.exitstatus != 0
			status = entry[:result] == true ? "OK".green : "FAIL".red
			printf "%s %s\n",
				"#{entry[:compiler]}/#{entry[:arch]}/#{entry[:ccflags].sub(' ', ',')}".brown,
				status
		end
	}
end

desc "Clean up stuff"
task :cleanup => [:execute] do
	system("rm -Rf ./build")
	if specs.index {|entry| entry[:result] == false} != nil then
		exit (1)
	else
		exit (0)
	end
end

def filename_for_spec(s)
	"#{s[:compiler]}_#{s[:arch]}#{s[:ccflags].delete(' ')}"
end

# Colors, Rainbows, Unicorns!
class String
	def red;   "\033[1;31m#{self}\033[0m" end
	def green; "\033[1;32m#{self}\033[0m" end
	def brown; "\033[0;33m#{self}\033[0m" end
end
