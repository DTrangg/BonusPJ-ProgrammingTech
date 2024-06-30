#include "HelperFunction.h"

// 1. Create a school year (2020-2021, for example)
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
    cout << "-> School year " << schoolYear << " created successfully." << endl;
}

// 2. Create several classes for 1st-year students
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
    cout << "-> Class " << className << " created successfully." << endl;
}

// 3. Add new 1st year students to 1st-year classes
void addNewStudentToClass(string& className, Student& student, string& schoolYear) {
    if (!isFirstYearClass(className, schoolYear)) {
        cout << "Class " << className << " is not a first-year class." << endl;
        return;
    }

    for (int i = 0; i < numOfClasses_; i++) {
        if (classes_[i].className == className) {
            Student* newStudents = new Student[classes_[i].numOfStudents + 1];
            for (int j = 0; j < classes_[i].numOfStudents; j++) {
                newStudents[j] = classes_[i].students[j];
            }
            newStudents[classes_[i].numOfStudents] = student;
            classes_[i].numOfStudents++;
            delete[] classes_[i].students;
            classes_[i].students = newStudents;
            cout << "Student " << student.studentId << " added to class " << className << " successfully." << endl;
            return;
        }
    }
    cout << "Class " << className << " not found." << endl;
}

// 4. Import a CSV file containing all students in a specific class
void importStudentsFromCSV(string& fileName, string& className, string& schoolYear) {
    if (!isFirstYearClass(className, schoolYear)) {
        cout << "-> Class " << className << " is not a 1st-year class." << endl;
        return;
    }

    ifstream file(fileName);
    if (!file) {
        cerr << "-> Could not open file " << fileName << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        string fields[6]; // student has 6 attributes
        splitCSVLine(line, fields, 6);
        Student student = { fields[0], fields[1], fields[2], fields[3], fields[4], fields[5] };
        addNewStudentToClass(className, student, schoolYear);
    }
    file.close();
    cout << "-> Students imported from " << fileName << " successfully." << endl;
}

// 5. No need to add 2nd-year, 3rd-year, or 4th-year students, because these students had been added in the previous years

// 6. Create a semester, the created one will be the current semester
void createSemester(string &semesterId, string &schoolYear, string &startDate, string &endDate)
{
    // Kiểm tra điều kiện học kỳ chỉ có thể là 1, 2, hoặc 3
    if (semesterId != "1" && semesterId != "2" && semesterId != "3")
    {
        cerr << "-> Invalid semester ID. Only 1, 2, or 3 are allowed." << endl;
        return;
    }

    for (int i = 0; i < numOfSemesters_; i++)
    {
        if (semesters_[i].schoolYear == schoolYear && semesters_[i].semesterId == semesterId)
        {
            cout << "-> Semester " << semesterId << " already exists for school year " << schoolYear << endl;
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

    cout << "-> Semester " << semesterId << " for school year " << schoolYear << " created successfully." << endl;
}

// 7. Add a course to this semester
void addNewCourseToSemester(Course newCourse)
{
    if (currentSemester == nullptr)
    {
        cerr << "-> No current semester available." << endl;
        return;
    }

    if (courseExists(newCourse.courseId) != -1)
    {
        cout << "-> Course " << newCourse.courseId << " has already existed." << endl;
        return;
    }

    newCourse.scores = new Score[newCourse.courseSize];

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

    cout << "-> Course " << newCourse.courseId << " added to this semester successfully." << endl;
}

// 8. Upload a CSV file containing a list of students enrolled in the course
void uploadStudentList(Course &course, const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "-> Cannot open file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        string studentId, studentName;
        getline(ss, studentId, ',');
        getline(ss, studentName, ',');

        Score newScore = {studentId, studentName, 0, 0, 0, 0};

        if (course.numOfStudents < course.courseSize)
        {
            course.scores[course.numOfStudents++] = newScore;
        }
        else
        {
            cout << "-> Course is full. Cannot add more students." << endl;
            break;
        }
    }

    file.close();
}

// 9.1. View a course
void viewCourse(Course course)
{
    int check = courseExists(course.courseId);
    if (check < 0)
    {
        cout << "-> Course " << course.courseId << " not found." << endl;
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
        cout << "-> No courses available." << endl;
        return;
    }

    cout << "-> List Of Courses:" << endl;
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

    cout << "Course " << course.courseId << " updated successfully." << endl;
}

// 11. Add a student to the course
void addStudentToCourse(Course &course, const Score &newStudent)
{
    if (course.numOfStudents < course.courseSize)
    {
        course.scores[course.numOfStudents++] = newStudent;
    }
    else
    {
        cout << "-> Course is full." << endl;
    }
}

// 11.1. Add a list of students to a course
void addStudentsToCourse(Course &course, Score *newStudents, int numOfStudents)
{
    if (course.courseSize - course.numOfStudents < numOfStudents)
    {
        cout << "-> The number of adding students exceeds the course size." << endl;
        return;
    }

    int i = 0;
    while (i < numOfStudents)
    {
        course.scores[course.numOfStudents] = newStudents[i++];
        course.numOfStudents++;
    }
    cout << "-> " << numOfStudents << " students have just enrolled in course " << course.courseId << endl;
}

// 12. Remove a student from the course
void removeStudentFromCourse(Course &course, const string &studentId)
{
    for (int i = 0; i < course.numOfStudents; ++i)
    {
        if (course.scores[i].studentId == studentId)
        {
            for (int j = i; j < course.numOfStudents - 1; ++j)
            {
                course.scores[j] = course.scores[j + 1];
            }
            --course.numOfStudents;
            cout << "-> Student " << studentId << " has been removed from course " << course.courseId << endl;
            break;
        }
    }
}

// 13. Delete a course
void deleteCourse(string courseID)
{
    if (currentSemester == nullptr)
    {
        cout << "-> No current semester available." << endl;
        return;
    }

    int num = currentSemester->numOfCourses;
    if (num == 0)
    {
        cout << "-> No courses available to delete." << endl;
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
        cout << "-> Course " << courseID << " not found." << endl;
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

    cout << "-> Course " << courseID << " deleted successfully." << endl;
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
        cout << "-> Student " << studentId << " is not registered for any courses this semester." << endl;
    }
}

// 15. View a list of classes
void viewListOfClasses()
{
    if (numOfClasses_ == 0)
    {
        cout << "-> No classes available." << endl;
        return;
    }

    cout << "-> List of Classes:" << endl;
    for (int i = 0; i < numOfClasses_; i++)
    {
        cout << "Class Name: " << classes_[i].className << endl;
        cout << "Number of Students: " << classes_[i].numOfStudents << endl;
        cout << endl;
    }
}

// 16. View a list of students in a class
void viewClassStudents(const Class &cls)
{
    cout << "-> List of students in " << cls.className << " class:" << endl;
    cout << "ID | Name:\n";
    for (int i = 0; i < cls.numOfStudents; ++i)
    {
        cout << cls.students[i].studentId << " | " << cls.students[i].firstName << " " << cls.students[i].lastName << endl;
    }
}

// 18. View a list of students in a course
void viewCourseStudents(const Course &course)
{
    cout << "-> List of students in course " << course.courseId << ":" << endl;
    cout << "ID | Name:\n";
    for (int i = 0; i < course.numOfStudents; ++i)
    {
        cout << course.scores[i].studentId << " | " << course.scores[i].studentName << endl;
    }
}

// 19. Export a list of students in a course to a CSV file
void exportCourseStudentList(const Course &course, const string &filename)
{
    ofstream file(filename);
    if (!file.is_open())
    {
        cerr << "-> Cannot open file: " << filename << endl;
        return;
    }

    file << "No,Student ID,Student Name\n";
    for (int i = 0; i < course.numOfStudents; ++i)
    {
        file << (i + 1) << "," << course.scores[i].studentId << "," << course.scores[i].studentName << "\n";
    }

    file.close();
    cout << "-> List of students in course " << course.courseId << " exported to file successfully." << endl;
}

// 20. Import the scoreboard of a course
void importScoreboard(string courseId, string fileName)
{
    int check = courseExists(courseId);
    if (check < 0)
    {
        cout << "-> Course " << courseId << " not found." << endl;
        return;
    }

    ifstream file(fileName);
    if (!file.is_open())
    {
        cerr << "-> Could not open file " << fileName << endl;
        return;
    }

    string line;
    getline(file, line); // Bỏ qua dòng tiêu đề

    int i = 0;
    Course course = currentSemester->courses[check];
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

void viewStudentScore(string courseId, string studentId);

// 21. View the scoreboard of a course
void viewCourseScoreboard(string courseId)
{
    int check = courseExists(courseId);
    if (check < 0)
    {
        cout << "-> Course " << courseId << " not found." << endl;
        return;
    }

    int num = currentSemester->courses[check].numOfStudents;
    for (int i = 0; i < num; i++)
    {
        viewStudentScore(courseId, currentSemester->courses[check].scores[i].studentId);
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

    cout << "Score of student " << score.studentId << " updated successfully." << endl;
}

// 23.1. Calculate a student GPA in a semester
double calculateSemesterGPA(const Student &student, const Semester &semester)
{
    double totalGradePoints = 0.0;
    int totalCredits = 0;
    for (int i = 0; i < semester.numOfCourses; i++)
    {
        Course &course = semester.courses[i];
        for (int j = 0; j < course.numOfStudents; j++)
        {
            if (course.scores[j].studentId == student.studentId)
            {
                double finalMark = course.scores[j].finalMark;
                totalGradePoints += finalMark * course.numOfCredits;
                totalCredits += course.numOfCredits;
            }
        }
    }
    if (totalCredits == 0)
        return 0.0;
    return totalGradePoints / totalCredits;
}

// 23.2. Calculate a student overall GPA
double calculateOverallGPA(const Student &student)
{
    double totalGradePoints = 0.0;
    int totalCredits = 0;
    for (int i = 0; i < numOfSemesters_; i++)
    {
        Semester &semester = semesters_[i];
        for (int j = 0; j < semester.numOfCourses; j++)
        {
            Course &course = semester.courses[j];
            for (int k = 0; k < course.numOfStudents; k++)
            {
                if (course.scores[k].studentId == student.studentId)
                {
                    double finalMark = course.scores[k].finalMark;
                    totalGradePoints += finalMark * course.numOfCredits;
                    totalCredits += course.numOfCredits;
                }
            }
        }
    }
    if (totalCredits == 0)
        return 0.0;
    return totalGradePoints / totalCredits;
}

// 23.3. View the scoreboard of a class
void viewClassScoreboard(string &className)
{
    for (int i = 0; i < numOfClasses_; i++)
    {
        if (classes_[i].className == className)
        {
            Class cls = classes_[i];
            cout << "-> Scoreboard for Class: " << className << endl;
            for (int j = 0; j < cls.numOfStudents; j++)
            {
                Student student = cls.students[j];
                cout << "Student ID: " << student.studentId << ", Name: " << student.firstName << " " << student.lastName << endl;
                for (int l = 0; l < currentSemester->numOfCourses; l++)
                {
                    Course course = currentSemester->courses[l];
                    for (int m = 0; m < course.numOfStudents; m++)
                    {
                        if (course.scores[m].studentId == student.studentId)
                        {
                            cout << "  + Course: " << course.courseName << ", Final Mark: " << course.scores[m].finalMark << endl;
                        }
                    }
                }

                double semesterGPA = calculateSemesterGPA(student, *currentSemester);
                double overallGPA = calculateOverallGPA(student);
                cout << "GPA for this semester: " << semesterGPA << endl;
                cout << "Overall GPA: " << overallGPA << endl;
                cout << endl;
            }
            return;
        }
    }
    cout << "-> Class " << className << " not found." << endl;
}

// 24. View a score of a student in a course
void viewStudentScore(string courseId, string studentId)
{
    int check = courseExists(courseId);
    if (check < 0)
    {
        cout << "-> Course " << courseId << " not found." << endl;
        return;
    }
    int check2 = isStudentInCourse(studentId, currentSemester->courses[check]);
    if (check2 < 0)
    {
        cout << "-> Student " << studentId << " has not enrolled in course " << courseId << endl;
        return;
    }

    Score score = currentSemester->courses[check].scores[check2];
    cout << score.studentId << " - " << score.studentName << ": ";
    cout << score.totalMark << " | " << score.finalMark << " | " << score.midtermMark << " | " << score.otherMark << endl;
}

// 24.1. View a scoreboard of a student in a semester
void viewStudentSemesterScoreboard(Semester semester, string studentId)
{
    for (int j = 0; j < semester.numOfCourses; j++) {
        Course course = semester.courses[j];
        if (isStudentInCourse(studentId, course)) {
            viewStudentScore(course.courseId, studentId);
        }
    }
}

// 24.2. View a scoreboard of a student in a semester, in a year, overall
void viewStudentScoreboard(string studentId, const string* semesterId = nullptr,
    const string* schoolYear = nullptr)
{
    if (semesterId == nullptr && schoolYear == nullptr) {
        for (int i = 0; i < numOfSemesters_; i++) {
            viewStudentSemesterScoreboard(semesters_[i], studentId);
        }
    }
    else if (semesterId == nullptr && schoolYear) {
        for (int i = 0; i < numOfSemesters_; i++) {
            if (semesters_[i].schoolYear == *schoolYear) {
                viewStudentSemesterScoreboard(semesters_[i], studentId);
            }
            if (semesters_[i].semesterId == "3")
                return;
        }
    }
    else if (semesterId && schoolYear) {
        for (int i = 0; i < numOfSemesters_; i++) {
            if (semesters_[i].semesterId == *semesterId && semesters_[i].schoolYear == *schoolYear) {
                viewStudentSemesterScoreboard(semesters_[i], studentId);
                return;
            }
        }
    }
}




/*----------------------------------------- FOR TESTING -----------------------------------------*/
void test_createSchoolYear()
{
    string schoolYear;
    cout << "\nEnter new school year: ";
    cin >> schoolYear;
    createSchoolYear(schoolYear);
}

void test_createClass()
{
    string className;
    cout << "\nEnter new class: ";
    cin >> className;
    createClass(className);
}

void test_importStudentsFromCSV()
{
    string className, schoolYear, fileName;
    cout << "\nEnter class name: ";
    cin >> className;
    cout << "Enter school year: ";
    cin >> schoolYear;
    cout << "Enter file name: ";
    cin >> fileName;
    importStudentsFromCSV(fileName, className, schoolYear);
}

void test_createSemester()
{
    string semesterId, schoolYear, startDate, endDate;
    cout << "\nEnter semester ID: ";
    cin >> semesterId;
    cout << "Enter school year: ";
    cin >> schoolYear;
    cout << "Enter start date (yyyy-mm-dd): ";
    cin >> startDate;
    cout << "Enter end date (yyyy-mm-dd): ";
    cin >> endDate;
    createSemester(semesterId, schoolYear, startDate, endDate);
}

void test_addNewCourseToSemester()
{
    Course newCourse;

    cout << "\nEnter course ID: ";
    cin >> newCourse.courseId;

    cin.ignore();
    cout << "Enter course name: ";
    getline(cin, newCourse.courseName);

    cout << "Enter class name: ";
    cin >> newCourse.className;

    cin.ignore();
    cout << "Enter teacher name: ";
    getline(cin, newCourse.teacher);

    cout << "Enter number of credits: ";
    cin >> newCourse.numOfCredits;

    cout << "Enter course size: ";
    cin >> newCourse.courseSize;

    newCourse.scores = nullptr;
    newCourse.numOfStudents = 0;

    cout << "Enter session (ex. MON/S1...): ";
    cin >> newCourse.session;
  
    addNewCourseToSemester(newCourse);
}

void test_uploadStudentsToCourse()
{
    /*
    Score studentScore1 = { "S001", "John", 0,0,0,0 };
    addStudentToCourse(currentSemester->courses[0], studentScore1);
    Score studentScore2 = { "S002", "Jane", 0,0,0,0 };
    addStudentToCourse(currentSemester->courses[0], studentScore2);
    Score studentScore3 = { "S003", "Michael", 0,0,0,0 };
    addStudentToCourse(currentSemester->courses[0], studentScore3);
    */

    string courseId, fileName;
    cout << "\nEnter course ID: ";
    cin >> courseId;

    int check = courseExists(courseId);
    if (check == -1) {
        cout << "-> Course " << courseId << " not found.\n";
        return;
    }

    cout << "Enter file name: ";
    cin >> fileName;
    
    uploadStudentList(currentSemester->courses[check], fileName);
}

void test_viewListOfCourses()
{
    viewListOfCourses();
}

void test_updateCourse() 
{
    string OldCourseId, input;
    string tempStr;
    int tempInt = 0;

    cout << "\nEnter course ID: ";
    cin >> OldCourseId;
    int check = courseExists(OldCourseId);
    if (check == -1) {
        cout << "-> Course " << OldCourseId << " not found.\n";
        return;
    }

    cout << "Enter new course ID (or leave empty to keep current): ";
    getline(cin, input);
    const string* courseId = input.empty() ? nullptr : &input;

    cout << "Enter new course name (or leave empty to keep current): ";
    getline(cin, input);
    const string* courseName = input.empty() ? nullptr : &input;

    cout << "Enter new class name (or leave empty to keep current): ";
    getline(cin, input);
    const string* className = input.empty() ? nullptr : &input;

    cout << "Enter new teacher name (or leave empty to keep current): ";
    getline(cin, input);
    const string* teacher = input.empty() ? nullptr : &input;

    cout << "Enter new number of credits (or leave empty to keep current): ";
    getline(cin, input);
    const int* numOfCredits = input.empty() ? nullptr : &tempInt;
    if (!input.empty()) {
        tempInt = stoi(input);
        numOfCredits = &tempInt;
    }

    cout << "Enter new course size (or leave empty to keep current): ";
    getline(cin, input);
    const int* courseSize = input.empty() ? nullptr : &tempInt;
    if (!input.empty()) {
        tempInt = stoi(input);
        courseSize = &tempInt;
    }

    cout << "Enter new session (or leave empty to keep current): ";
    getline(cin, input);
    const string* session = input.empty() ? nullptr : &input;

    // Update the course
    updateCourse(currentSemester->courses[check], courseId, courseName, className, teacher, numOfCredits, courseSize, session);
}

void test_removeStudentFromCourse()
{
    string courseId, studentId;
    cout << "\nEnter student ID: ";
    cin >> studentId;

    cout << "Enter course ID: ";
    cin >> courseId;
    int check = courseExists(courseId);
    if (check == -1) {
        cout << "-> Course " << courseId << " not found.\n";
        return;
    }

    removeStudentFromCourse(currentSemester->courses[check], studentId);
}

void test_deleteCourse()
{
    string courseId;
    cout << "\nEnter course ID: ";
    cin >> courseId;

    deleteCourse(courseId);
}

void test_viewStudentCourses()
{
    string studentId;
    cout << "\nEnter student ID: ";
    cin >> studentId;

    viewStudentCourses(studentId);
}

void test_viewListOfClasses()
{
    viewListOfClasses();
}

void test_viewClassStudents()
{
    string className;
    cout << "\nEnter class name: ";
    cin >> className;

    for (int i = 0; i < numOfClasses_; i++)
    {
        if (classes_[i].className == className)
        {
            viewClassStudents(classes_[i]);
            return;
        }
    }

    cout << "-> Class " << className << " not found." << endl;
}

void test_viewCourseStudents()
{
    string courseId;
    cout << "Enter course ID: ";
    cin >> courseId;
    int check = courseExists(courseId);
    if (check == -1) {
        cout << "-> Course " << courseId << " not found.\n";
        return;
    }

    viewCourseStudents(currentSemester->courses[check]);
}

void test_exportCourseStudents()
{
    string courseId, fileName;

    cout << "\nEnter course ID: ";
    cin >> courseId;
    int check = courseExists(courseId);
    if (check == -1) {
        cout << "-> Course " << courseId << " not found.\n";
        return;
    }

    cout << "Enter file name: ";
    cin >> fileName;

    exportCourseStudentList(currentSemester->courses[check], fileName);
}

void test_importScoreboard()
{
    string courseId, fileName;
    cout << "\nEnter course ID: ";
    cin >> courseId;
    cout << "Enter file name: ";
    cin >> fileName;
    importScoreboard(courseId, fileName);
}

void test_viewCourseScoreboard()
{
    string courseId;
    cout << "\nEnter course ID: ";
    cin >> courseId;
    viewCourseScoreboard(courseId);
}

void test_updateScore()
{
    string courseId, studentId, input;
    double mark = 0.0;

    cout << "\nEnter course ID: ";
    cin >> courseId;
    int check = courseExists(courseId);
    if (check == -1) {
        cout << "-> Course " << courseId << " not found.\n";
        return;
    }

    cout << "Enter student ID: ";
    cin >> studentId;
    int check2 = isStudentInCourse(studentId, currentSemester->courses[check]);
    if (check2 < 0)
    {
        cout << "-> Student " << studentId << " has not enrolled in course " << courseId << endl;
        return;
    }

    cout << "Enter new total mark (or leave empty to keep current): ";
    getline(cin, input);
    const double* totalMark = input.empty() ? nullptr : &mark;
    if (!input.empty()) {
        mark = stod(input);
        totalMark = &mark;
    }

    cout << "Enter new final mark (or leave empty to keep current): ";
    getline(cin, input);
    const double* finalMark = input.empty() ? nullptr : &mark;
    if (!input.empty()) {
        mark = stod(input);
        finalMark = &mark;
    }

    cout << "Enter new middle mark (or leave empty to keep current): ";
    getline(cin, input);
    const double* midtermMark = input.empty() ? nullptr : &mark;
    if (!input.empty()) {
        mark = stod(input);
        midtermMark = &mark;
    }

    cout << "Enter new other mark (or leave empty to keep current): ";
    getline(cin, input);
    const double* otherMark = input.empty() ? nullptr : &mark;
    if (!input.empty()) {
        mark = stod(input);
        otherMark = &mark;
    }

    // Update the score
    updateScore(currentSemester->courses[check].scores[check2], totalMark, finalMark, midtermMark, otherMark);
}

void test_calculateSemesterGPA()
{
    string studentId, semesterId, schoolYear;

    cout << "\nEnter student ID: ";
    cin >> studentId;
    Student student = {studentId, "", "", "", "", ""};

    cout << "\nEnter semester ID and school year: ";
    cin >> semesterId >> schoolYear;
    for (int i = 0; i < numOfSemesters_; i++) {
        if (semesters_[i].semesterId == semesterId && semesters_[i].schoolYear == schoolYear) {
            double gpa = calculateSemesterGPA(student, *currentSemester);
            cout << "-> Semester GPA for student " << studentId << ": " << gpa << endl;
            return;
        }
    }
    cout << "-> Semester " << semesterId << " in " << schoolYear << " not found." << endl;
}

void test_calculateOverallGPA()
{
    string studentId;
    cout << "\nEnter student ID: ";
    cin >> studentId;

    Student student = {studentId, "", "", "", "", ""};
    double gpa = calculateOverallGPA(student);
    cout << "-> Overall GPA for student " << studentId << ": " << gpa << endl;
}

void test_viewClassScoreboard()
{
    string className;
    cout << "\nEnter class name: ";
    cin >> className;

    viewClassScoreboard(className);
}

void test_viewStudentScoreboard()
{
    string studentId;
    cout << "\nEnter student ID: ";
    cin >> studentId;

    string input;

    cout << "Enter semester ID (or leave empty to view all): ";
    getline(cin, input);
    const string* semesterId = input.empty() ? nullptr : &input;

    cout << "Enter school year (or leave empty to view all): ";
    getline(cin, input);
    const string* schoolYear = input.empty() ? nullptr : &input;

    viewStudentScoreboard(studentId, semesterId, schoolYear);
}

void test_main_functions()
{
    cout << "\n------------------------ TESTING OPTIONS ------------------------";
    cout << "\n  -  1 - Create a school year (2020-2021, for example).";
    cout << "\n  -  2 - Create a class for 1st-year students.";
    cout << "\n  -  3 - Import new 1st year students from CSV file to 1st-year classes.";
    cout << "\n  -  4 - Create a semester, choose the school year that the semester belongs to.";
    cout << "\n  -  5 - Add a new course to a semester.";
    cout << "\n  -  6 - Import new students from CSV file to a course.";
    cout << "\n  -  7 - View the list of courses in the current semester.";
    cout << "\n  -  8 - Update a course information.";
    cout << "\n  -  9 - Remove a student from a course in the current semester.";
    cout << "\n  - 10 - Delete a course in the current semester.";
    cout << "\n  - 11 - View the list of courses that a student will study the current semester.";
    cout << "\n  - 12 - View the list of classes.";
    cout << "\n  - 13 - View the list of students in a class.";
    cout << "\n  - 14 - View the list of students in a course.";
    cout << "\n  - 15 - Export a list of students in a course to a CSV file.";
    cout << "\n  - 16 - Import the scoreboard of a course from a CSV file.";
    cout << "\n  - 17 - View the scoreboard of a course.";
    cout << "\n  - 18 - Update a student's result.";
    cout << "\n  - 19 - View the scoreboard of a class.";
    cout << "\n  - 20 - View a student's scoreboard.";
    cout << "\n--------------------------------------------------------------------------------";

    int key = 0;
    int choice = 0;

    do {
        cout << "\nMoi nhap lua chon: "; cin >> key;

        switch (key) {
            case 1: // Create a school year
                test_createSchoolYear();
                break;
            case 2: // Create a class
                test_createClass();
                break;
            case 3: // Import new 1st-year students from CSV file to a class
                test_importStudentsFromCSV();
                break;
            case 4: // Create a semester
                test_createSemester();
                break;
            case 5: // Add new course to a semester
                test_addNewCourseToSemester();
                break;
            case 6: // Upload list of students enrolled in the course from CSV file
                test_uploadStudentsToCourse();
                break;
            case 7: // View the list of courses
                test_viewListOfCourses();
                break;
            case 8: // Update a course
                test_updateCourse();
                break;
            case 9: // Remove a student from a course
                test_removeStudentFromCourse();
                break;
            case 10: // Delete a course
                test_deleteCourse();
                break;
            case 11: // View the list of courses that a student will study
                test_viewStudentCourses();
                break;
            case 12: // View the list of classes
                test_viewListOfClasses();
                break;
            case 13: // View the list of students in a class
                test_viewClassStudents();
                break;
            case 14: // View the list of students in a course
                test_viewCourseStudents();
                break;
            case 15: // Export a list of students in a course
                test_exportCourseStudents();
                break;
            case 16: // Import a course scoreboard from a CSV file
                test_importScoreboard();
                break;
            case 17: // View a course scoreboard
                test_viewCourseScoreboard();
                break;
            case 18: // Update a student's result
                test_updateScore();
                break;
            case 19: //  View a class scoreboard
                test_viewClassScoreboard();
                break;
            case 20: // View a student's scoreboard
                test_viewStudentScoreboard();
                break;
            default:
                cout << ">> Lua chon khong phu hop." << endl;
                break;
        }
        cout << "\nThoat chuong trinh (nhan -1)? ";
        cin >> key;
    } while (key != -1);

    cout << "\n>> Chuong trinh quan li ket thuc." << endl;
}

int main()
{
    test_main_functions();
    return 0;
}
