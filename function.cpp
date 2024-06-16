#include <header.h>
#include <fstream>

using namespace std;

// Biến toàn cục lưu trữ học kỳ
Semester* semesters_ = NULL;
int numOfSemesters_ = 0;
Semester* currentSemester = NULL;

Class* classes_;
int numOfClasses_;

// Hàm tạo năm học
void createSchoolYear(string& schoolYear) {
    Semester* newSemesters = new Semester[numOfSemesters_ + 1];
    for (int i = 0; i < numOfSemesters_; i++) {
        newSemesters[i] = semesters_[i];
    }
    newSemesters[numOfSemesters_].schoolYear = schoolYear;
    newSemesters[numOfSemesters_].numOfCourses = 0;
    newSemesters[numOfSemesters_].courses = NULL;
    numOfSemesters_++;
    delete[] semesters_;
    semesters_ = newSemesters;
    cout << "School year " << schoolYear << " created successfully." << endl;
}

// Hàm tạo lớp học cho sinh viên năm nhất
void createClass(string& className) {
    Class* newClasses = new Class[numOfClasses_ + 1];
    for (int i = 0; i < numOfClasses_; i++) {
        newClasses[i] = classes_[i];
    }
    newClasses[numOfClasses_].className = className;
    newClasses[numOfClasses_].numOfStudents = 0;
    newClasses[numOfClasses_].students = NULL;
    numOfClasses_++;
    delete[] classes_;
    classes_ = newClasses;
    cout << "Class " << className << " created successfully." << endl;
}

// Hàm thêm sinh viên năm nhất vào lớp năm nhất
void addNewStudentToClass(string& className, Student& student) {
    for (int i = 0; i < numOfClasses_; i++) {
        if (classes_[i].className == className) {
            Student* newStudents = new Student[classes_[i].numOfStudents + 1];
            for (int j = 0; j < classes_[i].numOfStudents; ++j) {
                newStudents[j] = classes_[i].students[j];
            }
            newStudents[classes_[i].numOfStudents] = student;
            classes_[i].numOfStudents++;
            delete[] classes_[i].students;
            classes_[i].students = newStudents;
            cout << "Student " << student.studentId << " added to class " << className << " successfully." << endl;
            
            // giả sử classname là duy nhất trong danh sách lớp
            return;
        }
    }
    cout << "Class " << className << " not found." << endl;
}

// Hàm tách chuỗi CSV thành các trường riêng biệt
void splitCSVLine(string& line, string fields[], int numFields) {
    int fieldIndex = 0;
    int start = 0;
    int end = line.find(',');
    while (end != string::npos && fieldIndex < numFields) {
        fields[fieldIndex++] = line.substr(start, end - start);
        start = end + 1;
        end = line.find(',', start);
    }
    if (fieldIndex < numFields) {
        fields[fieldIndex] = line.substr(start);
    }
}

// Hàm import CSV file chứa danh sách sinh viên vào hệ thống
void importStudentsFromCSV(string& fileName, string& className) {
    ifstream file(fileName);
    if (!file) {
        cout << "Could not open file " << fileName << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        string fields[6]; // student lưu 6 thuộc tính
        splitCSVLine(line, fields, 6);
        Student student = { fields[0], fields[1], fields[2], fields[3], fields[4], fields[5] };
        addNewStudentToClass(className, student);
    }
    file.close();
    cout << "Students imported from " << fileName << " successfully." << endl;
}

// Hàm tạo học kỳ
void createSemester(string& semesterId, string& schoolYear, string& startDate, string& endDate) {
    // Kiểm tra điều kiện học kỳ chỉ có thể là 1, 2, hoặc 3
    if (semesterId != "1" && semesterId != "2" && semesterId != "3") {
        cout << "Invalid semester ID. Only 1, 2, or 3 are allowed." << endl;
        return;
    }

    for (int i = 0; i < numOfSemesters_; i++) {
        if (semesters_[i].schoolYear == schoolYear && semesters_[i].semesterId == semesterId) {
            cout << "Semester " << semesterId << " already exists for school year " << schoolYear << endl;
            return;
        }
    }

    Semester* newSemesters = new Semester[numOfSemesters_ + 1];
    for (int i = 0; i < numOfSemesters_; i++) {
        newSemesters[i] = semesters_[i];
    }

    newSemesters[numOfSemesters_].semesterId = semesterId;
    newSemesters[numOfSemesters_].schoolYear = schoolYear;
    newSemesters[numOfSemesters_].startDate = startDate;
    newSemesters[numOfSemesters_].endDate = endDate;
    newSemesters[numOfSemesters_].numOfCourses = 0;
    newSemesters[numOfSemesters_].courses = NULL;
    numOfSemesters_++;

    delete[] semesters_;
    semesters_ = newSemesters;
    currentSemester = &semesters_[numOfSemesters_ - 1];

    cout << "Semester " << semesterId << " for school year " << schoolYear << " created successfully." << endl;
}

// Hàm hiển thị danh sách các lớp học
void viewListOfClasses() {
    if (numOfClasses_ == 0) {
        cout << "No classes available." << endl;
        return;
    }

    cout << "List of Classes:" << endl;
    for (int i = 0; i < numOfClasses_; i++) {
        cout << "Class Name: " << classes_[i].className << endl;
        cout << "Number of Students: " << classes_[i].numOfStudents << endl;
        cout << endl << endl;
    }
}
