#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

// Cấu trúc User (Người dùng)
struct User
{
    string userName; // tên tài khoản
    string passWord; // mật khẩu
    string fullName; // họ tên chủ tài tài khoản
    string role;     // vai trò: staff hoặc student
};

// Cấu trúc Student (Sinh viên)
struct Student
{
    string studentId;   // mã số sinh viên
    string firstName;   // tên của sinh viên
    string lastName;    // họ của sinh viên
    string gender;      // giới tính
    string dateOfBirth; // ngày sinh
    string socialId;    // mã định danh cá nhân
};

// Cấu trúc Class (Lớp học)
struct Class
{
    string className;  // tên lớp
    Student *students; // danh sách sinh viên
    int numOfStudents; // sỉ số
};

// Cấu trúc Score (Điểm số)
struct Score
{
    string studentId;   // mã số sinh viên
    string studentName; // tên sinh viên
    double totalMark;   // tổng điểm
    double finalMark;   // điểm cuối kỳ
    double midtermMark; // điểm giữa kỳ
    double otherMark;   // điểm quá trình, điểm cộng ...
};

// Cấu trúc Course (Học phần)
struct Course
{
    string courseId;   // mã học phần
    string courseName; // tên học phần
    string className;  // tên lớp
    string teacher;    // giáo viên phụ trách
    int numOfCredits;  // số tín chỉ
    int courseSize;    // số lượng sinh viên tối đa
    Score *scores;     // danh sách sinh viên tham gia
    int numOfStudents; // số lượng sinh viên đăng ký
    string session;    // buổi học (vd: MON/S1, TUE/S2 ...)
                       // (S1 (07:30), S2 (09:30), S3(13:30), S4 (15:30))
};

// Cấu trúc Semester (Học kỳ)
struct Semester
{
    string semesterId; // mã học kỳ: 1 (Fall), 2 (Spring), 3 (Summer)
    string schoolYear; // năm học
    string startDate;  // ngày bắt đầu
    string endDate;    // ngày kết thúc
    Course *courses;   // danh sách học phần trong học kỳ
    int numOfCourses;  // số lượng học phần
};