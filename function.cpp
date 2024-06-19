#include "Struct.h"

// Biến toàn cục lưu trữ học kỳ
Semester *semesters_ = NULL;
int numOfSemesters_ = 0;
Semester *currentSemester = NULL;

Class *classes_;
int numOfClasses_;

/*-----------------------------------------THY---------------------------------------------*/

// Hàm tạo năm học
void createSchoolYear(string &schoolYear)
{
    Semester *newSemesters = new Semester[numOfSemesters_ + 1];
    for (int i = 0; i < numOfSemesters_; i++)
    {
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
void createClass(string &className)
{
    Class *newClasses = new Class[numOfClasses_ + 1];
    for (int i = 0; i < numOfClasses_; i++)
    {
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
void addNewStudentToClass(string &className, Student &student)
{
    for (int i = 0; i < numOfClasses_; i++)
    {
        if (classes_[i].className == className)
        {
            Student *newStudents = new Student[classes_[i].numOfStudents + 1];
            for (int j = 0; j < classes_[i].numOfStudents; ++j)
            {
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
void splitCSVLine(string &line, string fields[], int numFields)
{
    int fieldIndex = 0;
    int start = 0;
    int end = line.find(',');
    while (end != string::npos && fieldIndex < numFields)
    {
        fields[fieldIndex++] = line.substr(start, end - start);
        start = end + 1;
        end = line.find(',', start);
    }
    if (fieldIndex < numFields)
    {
        fields[fieldIndex] = line.substr(start);
    }
}

// Hàm import CSV file chứa danh sách sinh viên vào hệ thống
void importStudentsFromCSV(string &fileName, string &className)
{
    ifstream file(fileName);
    if (!file)
    {
        cout << "Could not open file " << fileName << endl;
        return;
    }

    string line;
    while (getline(file, line))
    {
        string fields[6]; // student lưu 6 thuộc tính
        splitCSVLine(line, fields, 6);
        Student student = {fields[0], fields[1], fields[2], fields[3], fields[4], fields[5]};
        addNewStudentToClass(className, student);
    }
    file.close();
    cout << "Students imported from " << fileName << " successfully." << endl;
}

// Hàm tạo học kỳ
void createSemester(string &semesterId, string &schoolYear, string &startDate, string &endDate)
{
    // Kiểm tra điều kiện học kỳ chỉ có thể là 1, 2, hoặc 3
    if (semesterId != "1" && semesterId != "2" && semesterId != "3")
    {
        cout << "Invalid semester ID. Only 1, 2, or 3 are allowed." << endl;
        return;
    }

    for (int i = 0; i < numOfSemesters_; i++)
    {
        if (semesters_[i].schoolYear == schoolYear && semesters_[i].semesterId == semesterId)
        {
            cout << "Semester " << semesterId << " already exists for school year " << schoolYear << endl;
            return;
        }
    }

    Semester *newSemesters = new Semester[numOfSemesters_ + 1];
    for (int i = 0; i < numOfSemesters_; i++)
    {
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
void viewListOfClasses()
{
    if (numOfClasses_ == 0)
    {
        cout << "No classes available." << endl;
        return;
    }

    cout << "List of Classes:" << endl;
    for (int i = 0; i < numOfClasses_; i++)
    {
        cout << "Class Name: " << classes_[i].className << endl;
        cout << "Number of Students: " << classes_[i].numOfStudents << endl;
        cout << endl
             << endl;
    }
}

/*-----------------------------------------TRANG---------------------------------------------*/

// Hàm kiểm tra sự tồn tại của khóa học dựa trên courseID
int courseExists(string courseId)
{
    for (int i = 0; i < currentSemester->numOfCourses; i++)
    {
        if (currentSemester->courses[i].courseId == courseId)
            return i;
    }
    return -1;
}

// 7. Add a course to this semester
void addNewCourseToSemester(Course newCourse)
{
    if (currentSemester == nullptr)
    {
        cout << "No current semester available." << endl;
        return;
    }

    if (courseExists(newCourse.courseId) != -1)
    {
        cout << "Course " << newCourse.courseId << " has already existed." << endl;
        return;
    }

    int num = currentSemester->numOfCourses;
    Course *newCourses = new Course[num + 1];

    int i = 0;
    while (i < num)
    {
        newCourses[i] = currentSemester->courses[i];
        i++;
    }
    newCourses[i] = newCourse;
    currentSemester->numOfCourses++;

    delete[] currentSemester->courses;
    currentSemester->courses = newCourses;

    cout << "Course " << newCourse.courseId << " added to this semester successfully." << endl;
}

// 9.1. View a course
void viewCourse(Course course)
{
    int check = courseExists(course.courseId);
    if (check < 0)
    {
        cout << "Course " << course.courseId << " not found." << endl;
        return;
    }

    cout << "Course ID:         " << course.courseId << endl;
    cout << "Course Name:       " << course.courseName << endl;
    cout << "Class Name:        " << course.className << endl;
    cout << "Teacher:           " << course.teacher << endl;
    cout << "Number of Credits: " << course.numOfCredits << endl;
    cout << "Enrollment:        " << course.numOfStudents << "/" << course.courseSize << endl;
    cout << "Session:           " << course.session << endl;
}

// 9. View the list of courses (in this semester)
void viewListOfCourses()
{
    if (currentSemester->numOfCourses == 0)
    {
        cout << "No courses available." << endl;
        return;
    }

    cout << "List Of Courses:" << endl;
    for (int i = 0; i < currentSemester->numOfCourses; i++)
    {
        viewCourse(currentSemester->courses[i]);
        cout << endl;
    }
}

// 10. Update course information
void updateCourse(Course &course,
                  const string *courseId = nullptr,
                  const string *courseName = nullptr,
                  const string *className = nullptr,
                  const string *teacher = nullptr,
                  const int *numOfCredits = nullptr,
                  const int *courseSize = nullptr,
                  const string *session = nullptr)
{
    if (courseId)
        course.courseId = *courseId;
    if (courseName)
        course.courseName = *courseName;
    if (className)
        course.className = *className;
    if (teacher)
        course.teacher = *teacher;
    if (numOfCredits)
        course.numOfCredits = *numOfCredits;
    if (courseSize)
        course.courseSize = *courseSize;
    if (session)
        course.session = *session;
}

// 13. Delete a course
void deleteCourse(string courseID)
{
    if (currentSemester == nullptr)
    {
        cout << "No current semester available." << endl;
        return;
    }

    int num = currentSemester->numOfCourses;
    if (num == 0)
    {
        cout << "No courses available to delete." << endl;
        return;
    }

    Course *newCourses = new Course[num - 1];
    int i = 0;
    // Tìm khóa học cần xóa
    while (i < num && currentSemester->courses[i].courseId != courseID)
    {
        newCourses[i] = currentSemester->courses[i];
        i++;
    }
    // Nếu không tìm thấy khóa học
    if (i == num)
    {
        cout << "Course " << courseID << " not found." << endl;
        delete[] newCourses; // Giải phóng bộ nhớ đã cấp phát
        return;
    }
    // Di chuyển các khóa học còn lại
    while (i < num - 1)
    {
        newCourses[i] = currentSemester->courses[i + 1];
        i++;
    }

    delete[] currentSemester->courses;
    currentSemester->courses = newCourses;
    currentSemester->numOfCourses--;

    cout << "Course " << courseID << " deleted successfully." << endl;
}

// 14.1. Check if a student has enrolled in a course
int isStudentInCourse(string studentId, Course course)
{
    for (int i = 0; i < course.numOfStudents; ++i)
    {
        if (course.scores[i].studentId == studentId)
            return i;
    }
    return -1;
}

// 14. View a list of courses that a student will study this semester
void viewStudentCourses(string studentId)
{
    bool found = false;
    for (int i = 0; i < currentSemester->numOfCourses; ++i)
    {
        Course currentCourse = currentSemester->courses[i];
        for (int j = 0; j < currentCourse.numOfStudents; ++j)
        {
            if (isStudentInCourse(currentCourse.scores[j].studentId, currentCourse))
            {
                found = true;
                cout << currentCourse.courseId << ": " << currentCourse.courseName << " (" << currentCourse.session << ")" << endl;
                cout << endl;
                break;
            }
        }
    }

    if (!found)
    {
        cout << "Student " << studentId << " is not registered for any courses this semester." << endl;
    }
}

// 20. Import the scoreboard of a course
void importScoreboard(Course &course, string fileName)
{
    int check = courseExists(course.courseId);
    if (check < 0)
    {
        cout << "Course " << course.courseId << " not found." << endl;
        return;
    }

    ifstream file(fileName);
    if (!file.is_open())
    {
        cout << "Could not open file " << fileName << endl;
        return;
    }

    string line;
    getline(file, line); // Bỏ qua dòng tiêu đề

    int i = 0;
    while (getline(file, line))
    {
        stringstream ss(line);
        string token;

        getline(ss, course.scores[i].studentId, ',');   // studentId
        getline(ss, course.scores[i].studentName, ','); // studentName
        getline(ss, token, ',');                        // totalMark
        course.scores[i].totalMark = stod(token);
        getline(ss, token, ','); // finalMark
        course.scores[i].finalMark = stod(token);
        getline(ss, token, ','); // midtermMark
        course.scores[i].midtermMark = stod(token);
        getline(ss, token); // otherMark
        course.scores[i].otherMark = stod(token);

        i++;
    }
    file.close();
}

// 24. View a score of a student in a course
void viewScore(string courseId, string studentId)
{
    int check = courseExists(courseId);
    if (check < 0)
    {
        cout << "Course " << courseId << " not found." << endl;
        return;
    }
    int check2 = isStudentInCourse(studentId, currentSemester->courses[check]);
    if (check2 < 0)
    {
        cout << "Student " << studentId << " has not enrolled in course " << courseId << endl;
        return;
    }

    Score score = currentSemester->courses[check].scores[check2];
    cout << score.studentId << " - " << score.studentName << endl;
    cout << ": ";
    cout << score.totalMark << " | " << score.finalMark << " | " << score.midtermMark << " | " << score.otherMark << endl;
}

// 21. View the scoreboard of a course
void viewCourseScoreboard(string courseId)
{
    int check = courseExists(courseId);
    if (check < 0)
    {
        cout << "Course " << courseId << " not found." << endl;
        return;
    }

    int num = currentSemester->courses[check].numOfStudents;
    for (int i = 0; i < num; i++)
    {
        viewScore(courseId, currentSemester->courses[check].scores[i].studentId);
        cout << endl;
    }
}

// 22. Update a student's result
void updateScore(Score &score,
                 const double *totalMark = nullptr,
                 const double *finalMark = nullptr,
                 const double *midtermMark = nullptr,
                 const double *otherMark = nullptr)
{
    if (totalMark)
        score.totalMark = *totalMark;
    if (finalMark)
        score.finalMark = *finalMark;
    if (midtermMark)
        score.midtermMark = *midtermMark;
    if (otherMark)
        score.otherMark = *otherMark;
}


/*-----------------------------------------TRING---------------------------------------------*/
// Hàm đăng nhập
User* login(User* users, int numUsers, string username, string password) {
    for (int i = 0; i < numUsers; i++) {
        if (users[i].userName == username && users[i].passWord == password) {
            return &users[i];
        }
    }
    return NULL;
}

// Hàm xem thông tin hồ sơ người dùng
void viewProfile(User* user) {
    cout << "Username: " << user->userName << endl;
    cout << "Full Name: " << user->fullName << endl;
    cout << "Role: " << user->role << endl;
}

// Hàm thay đổi mật khẩu
void changePassword(User* user, string newPassword) {
    user->passWord = newPassword;
    cout << "Password changed successfully!" << endl;
}

// Hàm đăng xuất
void logout(User*& currentUser) {
    currentUser = NULL;
    cout << "Logged out successfully!" << endl;
}


