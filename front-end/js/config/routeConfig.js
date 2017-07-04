// Route provider to feed the single page application. Pages can be accessed by the path indicated after 'when'

angular.module("ruServer").config(function ($routeProvider) {

    $routeProvider
        .when("/login", {
            templateUrl: "view/login.html",
            controller: "loginCtrl"
        })
        .when("/admin", {
            templateUrl: "view/admin/admin-home.html",
            controller: "adminHomeCtrl"
        })
        .when("/admin/manage-credits", {
            templateUrl: "view/admin/admin-manage-credits.html",
            controller: "adminManageCreditsCtrl"
        })
        .when("/admin/manage-users", {
            templateUrl: "view/admin/admin-manage-users.html",
            controller: "adminManageUsersCtrl"
        })
        .when("/admin/manage-users/edit-user", {
            templateUrl: "view/admin/admin-edit-user.html",
            controller: "adminEditUserCtrl"
        })
        .when("/admin/log", {
            templateUrl: "view/admin/admin-view-log.html",
            controller: "adminLogCtrl"
        })
        .when("/student", {
            templateUrl: "view/student/student-home.html",
            controller: "studentHomeCtrl"
        })
        .when("/student/add-credits", {
            templateUrl: "view/student/student-add-credits.html",
            controller: "studentAddCreditsCtrl"
        })
        .when("/student/log", {
            templateUrl: "view/student/student-view-log.html",
            controller: "studentLogCtrl"
        })
        .when("/student/change-password", {
            templateUrl: "view/student/student-change-password.html",
            controller: "studentChangePasswordCtrl"
        })
        .when("/error", {
            templateUrl: "view/error.html",
            controller: "errorCtrl"
        })
        .otherwise({ redirectTo: "/login" }); // If path not found, it loads automatically the login page
});