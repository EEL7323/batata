// Declaration of the errorInterceptor, used to redirect the user to the error page when some file is missing (404 error)
angular.module("ruServer").factory("errorInterceptor", function ($q, $location) {
    return {
        responseError: function (rejection) {
            if (rejection.status === 404) {
                $location.path("/error");
            }
            return $q.reject(rejection);
        }
    };
});
