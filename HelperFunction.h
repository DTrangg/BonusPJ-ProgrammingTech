#include "Header.h"

// Log in to the system
bool login(User* users, int numOfUsers, const string& username, const string& password) {
    for (int i = 0; i < numOfUsers; ++i) {
        if (users[i].userName == username && users[i].passWord == password) {
            return true;
        }
    }
    return false;
}

// Log out of the system
void logout(bool& isLoggedIn) {
    isLoggedIn = false;
}

// View a user's profile info
User* viewProfile(User* users, int numOfUsers, const string& username) {
    for (int i = 0; i < numOfUsers; ++i) {
        if (users[i].userName == username) {
            return &users[i];
        }
    }
    return nullptr;
}

// Change a user's password
bool changePassword(User* users, int numOfUsers, const string& username, const string& newPassword) {
    for (int i = 0; i < numOfUsers; ++i) {
        if (users[i].userName == username) {
            users[i].passWord = newPassword;
            return true;
        }
    }
    return false;
}

// Split a line into an array
void splitCSVLine(string& line, string fields[], int numFields)
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

// Check if a course available
int courseExists(string courseId)
{
    for (int i = 0; i < currentSemester->numOfCourses; i++)
    {
        if (currentSemester->courses[i].courseId == courseId)
            return i;
    }
    return -1;
}

// Check if a student has enrolled in a course
int isStudentInCourse(string studentId, Course course)
{
    for (int i = 0; i < course.numOfStudents; ++i)
    {
        if (course.scores[i].studentId == studentId)
            return i;
    }
    return -1;
}