# ชื่อไฟล์ output
TARGET = GG

# คอมไพล์เลอร์และแฟล็ก
CXX = g++
CXXFLAGS = -std=c++17 -Wall -I. -Iyok -IGamyui

# ไฟล์ต้นทางหลัก (main)
SRC = Gamyui/main.cpp

# เพิ่ม .cpp อื่นๆ ตามต้องการ เช่น jobapp.cpp, user.cpp ฯลฯ
# เช่น:
# SRC += jobapp.cpp company.cpp jobseeker_company.cpp

# สร้าง object list จาก .cpp
OBJ = $(SRC:.cpp=.o)

# กฎหลัก
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# สั่ง clean ลบไฟล์ชั่วคราว
clean:
	rm -f $(OBJ) $(TARGET)
