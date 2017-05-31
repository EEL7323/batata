angular.module("ruServer").factory("loadingInterceptor", function ($q, $rootScope, $timeout) {
    return {
        request: function (config) {
            if (config.url == "view/login.html") {
                $rootScope.loading = true;
            }
            return config;
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
