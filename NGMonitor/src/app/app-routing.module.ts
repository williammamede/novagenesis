import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { GraphAppComponent } from './graph-app/graph-app.component';
import { BindingsComponent } from './bindings/bindings.component';
import { DataMonitorComponent } from './data-monitor/data-monitor.component';
import { LifecycleComponent } from './lifecycle/lifecycle.component';

const routes: Routes = [
  {path: 'lifeCycle', component: LifecycleComponent},
  {path: 'bindings', component: BindingsComponent},
  {path: 'nameResolution', component: GraphAppComponent},
  {path: 'dataTransfer', component: DataMonitorComponent},
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
