// Declaration of the interceptor loadingInterceptor, used to show a loading message when the user logs in
angular.module("ruServer").factory("loadingInterceptor", function ($q, $rootScope, $timeout) {
    return {
        request: function (config) {
            if (config.url == "view/login.html") {
                $rootScope.loading = true;
                $rootScope.loadingText = "Loading, please wait...";
                $rootScope.loadAgain = true;
            }
            else if (config.url == "view/admin/admin-home.html" || config.url == "view/student/student-home.html") {
                $rootScope.loadAgain = true;
            }
            return config;
            $rootScope.loadAgain = false;
        },
        requestError: function (rejection) {
            $rootScope.loading = false;
            return $q.reject(rejection);
        },
        response: function (response) {
            $timeout(function () {
                $rootScope.loading = false;
            }, 1000);
            return response;
        },
        responseError: function (rejection) {
            $rootScope.loading = false;
            return $q.reject(rejection);
        }
    };
});
