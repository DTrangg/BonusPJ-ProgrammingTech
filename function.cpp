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


/*-----------------------------------------TRINH---------------------------------------------*/

// Đăng nhập vào hệ thống
bool login(User* users, int numOfUsers, const string& username, const string& password) {
    for (int i = 0; i < numOfUsers; ++i) {
        if (users[i].userName == username && users[i].passWord == password) {
            return true;
        }
    }
    return false;
}

// Xem thông tin hồ sơ của User
User* viewProfile(User* users, int numOfUsers, const string& username) {
    for (int i = 0; i < numOfUsers; ++i) {
        if (users[i].userName == username) {
            return &users[i];
        }
    }
    return nullptr;
}

// Thay đổi mật khẩu
bool changePassword(User* users, int numOfUsers, const string& username, const string& newPassword) {
    for (int i = 0; i < numOfUsers; ++i) {
        if (users[i].userName == username) {
            users[i].passWord = newPassword;
            return true;
        }
    }
    return false;
}

// Đăng xuất khỏi hệ thống
void logout(bool& isLoggedIn) {
    isLoggedIn = false;
}

// Tải lên tệp CSV chứa danh sách học viên đã đăng ký khóa học
void uploadStudentList(Course& course, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Cannot open file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string studentId, studentName;
        getline(ss, studentId, ',');
        getline(ss, studentName, ',');

        Score newScore = {studentId, studentName, 0, 0, 0, 0};

        if (course.numOfStudents < course.courseSize) {
            course.scores[course.numOfStudents++] = newScore;
        } else {
            cerr << "Course is full. Cannot add more students." << endl;
            break;
        }
    }

    file.close();
}

// Thêm học viên vào khóa học
void addStudentToCourse(Course& course, const Score& newStudent) {
    if (course.numOfStudents < course.courseSize) {
        course.scores[course.numOfStudents++] = newStudent;
    } else {
        cout << "Course is full." << endl;
    }
}

// Loại bỏ học sinh khỏi khóa học
void removeStudentFromCourse(Course& course, const string& studentId) {
    for (int i = 0; i < course.numOfStudents; ++i) {
        if (course.scores[i].studentId == studentId) {
            for (int j = i; j < course.numOfStudents - 1; ++j) {
                course.scores[j] = course.scores[j + 1];
            }
            --course.numOfStudents;
            break;
        }
    }
}

// Xem danh sách học sinh trong lớp
void viewStudentList(const Class& cls) {
    for (int i = 0; i < cls.numOfStudents; ++i) {
        cout << "ID: " << cls.students[i].studentId << ", Name: " << cls.students[i].firstName << " " << cls.students[i].lastName << endl;
    }
}

// Xem danh sách học viên của một khóa học
void viewCourseStudents(const Course& course) {
    for (int i = 0; i < course.numOfStudents; ++i) {
        cout << "ID: " << course.scores[i].studentId << ", Name: " << course.scores[i].studentName << endl;
    }
}

// Xuất danh sách sinh viên của một khóa học sang file CSV
void exportCourseStudentList(const Course& course, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Cannot open file: " << filename << endl;
        return;
    }

    file << "No,Student ID,Full Name\n";
    for (int i = 0; i < course.numOfStudents; ++i) {
        file << (i + 1) << "," << course.scores[i].studentId << "," << course.scores[i].studentName << "\n";
    }

    file.close();
}


